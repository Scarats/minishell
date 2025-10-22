/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:53:25 by aadeikal          #+#    #+#             */
/*   Updated: 2025/10/22 16:53:28 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static volatile sig_atomic_t g_hd_interrupted = 0;

static char	*heredoc_filename(void)
{
	static int	i = 0;
	char		*filename;
	char		*num_str;
	char		*tmp;

	num_str = ft_itoa(i++);
	if (!num_str)
		return (NULL);
	tmp = ft_strjoin("/tmp/minishell_heredoc_", num_str);
	free(num_str);
	if (!tmp)
		return (NULL);
	filename = ft_strjoin(tmp, ".tmp");
	free(tmp);
	return (filename);
}

static void	heredoc_signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        g_hd_interrupted = 1;
        /* async-signal-safe */
        write(STDOUT_FILENO, "\n", 1);
    }
}

static int	setup_child_process(int fd)
{
    struct sigaction	sa_int;
    struct sigaction	sa_old;

    sa_int.sa_handler = heredoc_signal_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, &sa_old);
    if (fd == -1)
        exit(1);
    return (fd);
}

// Replace readline loop with get_next_line to avoid terminal corruption on SIGINT
static void	read_heredoc_input(int fd, char *delimiter)
{
    char	*line;
    size_t	len;

    g_hd_interrupted = 0;
    while (1)
    {
        fdprintf(STDOUT_FILENO, "heredoc> ");
        line = get_next_line(STDIN_FILENO);
        if (!line)
            break;
        len = ft_strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            line = NULL;
            break;
        }
        fdprintf(fd, "%s\n", line);
        free(line);
        line = NULL;
    }
    close(fd);
    if (g_hd_interrupted)
        exit(130);
    exit(0);
}

static int	handle_parent_process(pid_t pid, char *filename, t_redir *redir,
		t_main_data *data, struct sigaction *sa_old_int)
{
    int	status;
     int	wret;
     int	fd;

     // Retry if waitpid is interrupted by SIGINT in the parent
     do {
         wret = waitpid(pid, &status, 0);
     } while (wret == -1 && errno == EINTR);
     if (wret == -1)
     {
         // Could not reap child; treat as interrupted
         unlink(filename);
         free(filename);
         data->root->last_exit_status = 130;
		/* restore parent's SIGINT handler */
		sigaction(SIGINT, sa_old_int, NULL);
		return (-1);
     }
     if (WIFSIGNALED(status))
     {
         int sig = WTERMSIG(status);
         unlink(filename);
         free(filename);
         data->root->last_exit_status = (sig == SIGINT) ? 130 : 128 + sig;
		sigaction(SIGINT, sa_old_int, NULL);
		return (-1);
     }
     if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
     {
         unlink(filename);
         free(filename);
         data->root->last_exit_status = WEXITSTATUS(status);

		sigaction(SIGINT, sa_old_int, NULL);
		return (-1);
     }
     fd = open(filename, O_RDONLY);
     if (fd == -1)
     {
         fdprintf(2, "minishell: %s: %s\n", filename, strerror(errno));
         free(filename);
		sigaction(SIGINT, sa_old_int, NULL);
        return (-1);
     }
     redir->filename = my_strdup(&data->root->list_of_list, filename);
     free(filename);
	/* restore parent's SIGINT handler */
	sigaction(SIGINT, sa_old_int, NULL);
    return (fd);
}

int	heredoc(t_redir *redir, t_main_data *data)
{
    char	*filename;
     pid_t	pid;
     int		fd;
	struct sigaction sa_ign;
	struct sigaction sa_old_int;

     filename = heredoc_filename();
     if (!filename)
         return (fdprintf(2, "minishell: allocation error\n"), -1);
	/* parent ignores SIGINT during heredoc */
	sa_ign.sa_handler = SIG_IGN;
	sigemptyset(&sa_ign.sa_mask);
	sa_ign.sa_flags = 0;
	sigaction(SIGINT, &sa_ign, &sa_old_int);
     pid = fork();
     if (pid == -1)
	{
		/* restore handler on error path */
		sigaction(SIGINT, &sa_old_int, NULL);
		return (free(filename), fdprintf(2, "minishell: fork error\n"), -1);
	}
     if (pid == 0)
     {
         fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
         fd = setup_child_process(fd);
         read_heredoc_input(fd, redir->filename);
     }

	return (handle_parent_process(pid, filename, redir, data, &sa_old_int));
}
