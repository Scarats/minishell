/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 17:44:49 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/14 18:20:34 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*generate_heredoc_filename(t_main_data *data)
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

static void	heredoc_child_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		exit(130);
	}
	if (sig == SIGQUIT)
		return ;
}

static void	attach_tty_for_readline(void)
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

static int	create_heredoc_file(char **filename, t_main_data *data)
{
	int	fd;

	*filename = generate_heredoc_filename(data);
	if (!*filename)
		return (-1);
	fd = open(*filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(*filename);
		return (-1);
	}
	return (fd);
}
