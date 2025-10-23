/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:06:50 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/23 13:07:43 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_stop_flag = 0;

void	handle_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		g_stop_flag = 1;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		write(STDOUT_FILENO, "minishell: quit (core dumped)\n", 31);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static int	process_command(t_main_data *data, char *line)
{
	reset_tokenizer_for_line(data->tok, line);
	if (!parser(data))
		data->root->last_exit_status = traverse_tree(data->node, data);
	else
		return (1);
	return (data->root->last_exit_status);
}

bool	execute_command_loop(t_main_data *data, char *prompt)
{
	char	*line;

	line = NULL;
	fflush(stdout);
	line = readline(prompt);
	if (!line)
	{
		handle_eof(&line);
		return (false);
	}
	if (line[0] == '\0')
	{
		handle_empty_input(&line);
		return (true);
	}
	if (!ft_strncmp(line, "exit", 5))
	{
		free(line);
		line = NULL;
		return (false);
	}
	add_history(line);
	process_command(data, line);
	cleanup_after_command(data, &line);
	return (true);
}

int	main(int ac, char **av, char **envp)
{
	t_main_data	data;
	t_root		root;
	char		prompt[1024];
	int			exit_status;

	ft_memset(&root, 0, sizeof(root));
	ft_memset(&data, 0, sizeof(data));
	(void)ac;
	(void)av;
	exit_status = 0;
	root.malloc_root = NULL;
	root.env = NULL;
	root.list_of_list = NULL;
	root.data = &data;
	data.root = &root;
	root.env = set_env_var_list(&root, envp);
	if (initialize_shell(&data, prompt, sizeof(prompt)))
		return (1);
	while (execute_command_loop(&data, prompt))
		;
	exit_status = root.last_exit_status;
	cleanup(&data, &root);
	ft_lstclear(&root.list_of_list, NULL);
	my_free(&root.malloc_root);
	return (exit_status);
}
