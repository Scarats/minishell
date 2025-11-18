/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 19:54:26 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/18 16:01:57 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *generate_heredoc_filename(t_main_data *data)
{
	static int counter = 0;
	char *filename;
	char *tmp;

	tmp = ft_itoa(counter++);
	if (!tmp)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", tmp);
	my_addtolist(&data->malloc_tree, filename);
	free(tmp);
	return (filename);
}

void heredoc_child_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("sigint\n");
		g_stop_flag = 1;
		write(STDOUT_FILENO, "\n", 1);
		rl_done = 1;
		rl_replace_line("", 0);
        rl_on_new_line();
		//g_stop_flag = 130;

		// /exit(130);
	}
	if (sig == SIGQUIT)
		return;
}

void attach_tty_for_readline(void)
{
	int tty;

	tty = open("/dev/tty", O_RDWR);
	if (tty >= 0)
	{
		dup2(tty, STDIN_FILENO);
		dup2(tty, STDOUT_FILENO);
		close(tty);
	}
}

int is_delimiter_match(char *line, char *delimiter)
{
	size_t delim_len;

	if (!delimiter)
		return (0);
	delim_len = ft_strlen(delimiter);
	if (ft_strncmp(line, delimiter, delim_len) == 0 && line[delim_len] == '\0')
		return (1);
	return (0);
}

int read_heredoc_input(int fd, char *delimiter, t_root *root)
{
	char *line;
	(void)root;

	while (1)
    {
		printf("i\n");
        line = readline("> ");
        if (g_stop_flag)
        {
			printf("g_stop_flag\n");
            free(line);
            return (130);
        }
        if (!line)
		{
            printf("line\n");
			return (0);
		}
        if (is_delimiter_match(line, delimiter))
        {
			printf("delimiter\n");
            free(line);
            return (0);
        }
		printf("no ifs\n");
        ft_putendl_fd(line, fd);
        free(line);
    }

	/* while (1)
	{
		printf("i");
		line = readline("> ");
		if (g_stop_flag)
		{
			printf(RED"code 130\n"RESET);
			close(fd);
			my_multi_free(&root->list_of_list);
			if (root->data->fd_backup.in >= 0)
			{
				close(root->data->fd_backup.in);
				root->data->fd_backup.in = -1;
			}
			if (root->data->fd_backup.out >= 0)
			{
				close(root->data->fd_backup.out);
				root->data->fd_backup.out = -1;
			}
			break ;
		}
		if (!line)
			break;
		if (is_delimiter_match(line, delimiter))
		{
			free(line);
			break;
		}
		ft_putendl_fd(line, fd);
		free(line);
	} */
	/* if (g_stop_flag == 130)
		return (130); */
	
	return (0);
}
