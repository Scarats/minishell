/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 19:54:26 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/14 20:00:20 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*generate_heredoc_filename(t_main_data *data)
{
	static int	counter = 0;
	char		*filename;
	char		*tmp;

	tmp = ft_itoa(counter++);
	if (!tmp)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", tmp);
	my_addtolist(&data->malloc_tree, filename);
	free(tmp);
	return (filename);
}

void	heredoc_child_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		exit(130);
	}
	if (sig == SIGQUIT)
		return ;
}

void	attach_tty_for_readline(void)
{
	int	tty;

	tty = open("/dev/tty", O_RDWR);
	if (tty >= 0)
	{
		dup2(tty, STDIN_FILENO);
		dup2(tty, STDOUT_FILENO);
		close(tty);
	}
}

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
