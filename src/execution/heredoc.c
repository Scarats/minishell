/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:53:25 by aadeikal          #+#    #+#             */
/*   Updated: 2025/10/23 16:26:10 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
        //fdprintf(STDERR_FILENO, "DEBUG: Heredoc child received SIGINT\n");
        stop_flag = 1;
        write(STDOUT_FILENO, "\n", 1);
        // No _exit here - let the loop detect stop_flag
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

    stop_flag = 0; // Reset at start
    while (!stop_flag) // Check global flag
    {
        fdprintf(STDOUT_FILENO, "heredoc> ");
        line = get_next_line(STDIN_FILENO);
        if (!line)
            break;
        // Check if SIGINT was received while waiting for input
        if (stop_flag)
        {
            free(line);
            break;
        }
        len = ft_strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        fdprintf(fd, "%s\n", line);
        free(line);
    }
    close(fd);
    // Exit based on whether we were interrupted
    if (stop_flag)
    {
        //fdprintf(STDERR_FILENO, "DEBUG: Child exiting with 130 due to SIGINT, stop_flag=%d\n", stop_flag);
        exit(130);
    }
    //fdprintf(STDERR_FILENO, "DEBUG: Child exiting normally with 0\n");
    exit(0);
}

static int	handle_parent_process(pid_t pid, char *filename, t_redir *redir,
        t_main_data *data, struct sigaction *sa_old_int)
{
    int	status;
    int	wret;
    int	fd;

    wret = waitpid(pid, &status, 0);
    while (wret == -1 && errno == EINTR)
        wret = waitpid(pid, &status, 0);
    fdprintf(STDERR_FILENO, "DEBUG: heredoc parent status=%d\n", data->root->last_exit_status);
    if (wret == -1)
    {
        // Could not reap child; treat as interrupted
        unlink(filename);
        free(filename);
        data->root->last_exit_status = 130;
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
    // Success path - reset last_exit_status to 0 explicitly
    //data->root->last_exit_status = 0;
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        free(filename);
        sigaction(SIGINT, sa_old_int, NULL);
        return (-1);
    }
    redir->filename = my_strdup(&data->root->list_of_list, filename);
    free(filename);
    sigaction(SIGINT, sa_old_int, NULL);
    return (fd);
}

// A new function for non-forking heredoc
int heredoc_no_fork(t_redir *redir, t_main_data *data)
{
    char *filename = heredoc_filename();
    if (!filename)
        return (-1);
    
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1) {
        free(filename);
        return (-1);
    }
    
    // Set up signal handlers directly
    struct sigaction sa_int, sa_old_int;
    sa_int.sa_handler = heredoc_signal_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, &sa_old_int);
    
    // Read heredoc content directly
    read_heredoc_input_no_fork(fd, redir->filename);
    
    // Restore signal handler
    sigaction(SIGINT, &sa_old_int, NULL);
    
    // If interrupted, propagate 130
    if (stop_flag) {
        data->root->last_exit_status = 130;
        unlink(filename);
        free(filename);
        close(fd);
        return (-1);
    }
    
    // Success path
    redir->filename = my_strdup(&data->root->list_of_list, filename);
    int read_fd = open(filename, O_RDONLY);
    free(filename);
    return read_fd;
}

int	heredoc(t_redir *redir, t_main_data *data)
{
    char	*filename;
    pid_t	pid;
    int	fd;
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
