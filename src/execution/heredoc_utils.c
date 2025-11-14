/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 17:38:03 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/14 17:38:07 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_delimiter_match(char *line, char *delimiter)
{
	size_t	delim_len;

	if (!delimiter)
		return (0);
	delim_len = ft_strlen(delimiter);
	if (ft_strncmp(line, delimiter, delim_len) == 0 && line[delim_len] == '\0')
		return (1);
	return (0);
}

int	read_heredoc_input(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (is_delimiter_match(line, delimiter))
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	return (0);
}

void	setup_child_signals(void)
{
	struct sigaction	sa_new;

	sa_new.sa_handler = heredoc_child_signal_handler;
	sigemptyset(&sa_new.sa_mask);
	sa_new.sa_flags = 0;
	sigaction(SIGINT, &sa_new, NULL);
	sigaction(SIGQUIT, &sa_new, NULL);
}

int	handle_child_process(int fd, t_redir *redir, t_main_data *data)
{
	t_root	*root;

	root = data->root;
	setup_child_signals();
	attach_tty_for_readline();
	read_heredoc_input(fd, redir->filename);
	close(fd);
	my_multi_free(&root->list_of_list);
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
