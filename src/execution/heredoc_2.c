/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 19:53:46 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/20 14:18:35 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_child_signals(void)
{
    struct sigaction	sa_new;
    struct sigaction	sa_quit;

    sa_new.sa_handler = heredoc_child_signal_handler;
    sigemptyset(&sa_new.sa_mask);
    sa_new.sa_flags = 0;
    sigaction(SIGINT, &sa_new, NULL);
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}

static void	close_backup_fd(int *fd, int std_fd)
{
	if (*fd >= 0 && *fd != std_fd)
	{
		close(*fd);
		*fd = -1;
	}
}

int	handle_child_process(int fd, t_redir *redir, t_main_data *data)
{
	t_root	*root;

	root = data->root;
	setup_child_signals();
	attach_tty_for_readline();
	read_heredoc_input(fd, redir->filename, root);
	close(fd);
	close_backup_fd(&data->fd_backup.in, STDIN_FILENO);
	close_backup_fd(&data->fd_backup.out, STDOUT_FILENO);
	my_multi_free(&root->list_of_list);
	if (g_stop_flag)
		exit(130);
	exit(0);
}

int	handle_wait_status(int status, t_main_data *data, char *filename)
{
	t_root	*root;
	int		exit_code;

	root = data->root;
	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		if (exit_code == 130)
		{
			root->last_exit_status = 130;
			g_stop_flag = 1;
			unlink(filename);
			return (-1);
		}
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		root->last_exit_status = 130;
		g_stop_flag = 1;
		unlink(filename);
		return (-1);
	}
	return (0);
}

int	create_heredoc_file(t_main_data *data, char **filename)
{
	int	fd;

	*filename = generate_heredoc_filename(data);
	if (!*filename)
		return (-1);
	fd = open(*filename, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0600);
	if (fd == -1)
		return (-1);
	return (fd);
}
