/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:06:50 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/10 14:48:06 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_stop_flag = 0;

void	handle_signals_rl(void)
{
    struct sigaction	sa_int;
    struct sigaction	sa_quit;

    sa_int.sa_handler = handler_rl;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);
    
    sa_quit.sa_handler = handler_rl;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void	handle_signals(void)
{
    struct sigaction	sa_int;
    struct sigaction	sa_quit;

    sa_int.sa_handler = handler;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);
    
    sa_quit.sa_handler = handler;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void	handle_signals_heredoc(void)
{
    struct sigaction	sa_int;
    struct sigaction	sa_quit;

    sa_int.sa_handler = handler_heredoc;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);
    
    sa_quit.sa_handler = handler;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void	handler_rl(int sig)
{
    if (sig == SIGINT)
    {
        g_stop_flag = 1;
        rl_on_new_line();
        rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
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

void	handler(int sig)
{
    if (sig == SIGINT)
    {
        g_stop_flag = 1;
		write(STDOUT_FILENO, "\n", 1);
    }
    else if (sig == SIGQUIT)
    {
        write(STDOUT_FILENO, "minishell: quit (core dumped)\n", 31);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void	handler_heredoc(int sig)
{
    if (sig == SIGINT)
    {
        g_stop_flag = 1;
        write(STDOUT_FILENO, "\n", 1);
        exit(130);  // Exit with proper code
    }
}

int	process_command(t_main_data *data, char *line)
{
	t_root	*root;

	root = data->root;
	reset_tokenizer_for_line(data->tok, line);
	if (g_stop_flag)
	{
		root->last_exit_status = 130;
		g_stop_flag = 0;
		return (1);
	}
	if (!parser(data))
		root->last_exit_status = traverse_tree(data->node, data);
/* 	if (process_heredocs(data)) 
	{
    	root->last_exit_status = 130;  // Propagate status
    	return root->last_exit_status;
	} */
	else
	{
		if(!g_stop_flag)
			root->last_exit_status = 1;
		return (1);
	}
	return (root->last_exit_status);
}

bool	execute_command_loop(t_root *root, char *prompt)
{
    char *line;
    int ttyfd;

    g_stop_flag = 0;
    handle_signals_rl();
    line = readline(prompt);
    
    if (g_stop_flag)
    {
        root->last_exit_status = 130;
        g_stop_flag = 0;
        if (line)
            free(line);
        
        // Restore terminal state
        ttyfd = open("/dev/tty", O_RDONLY);
        if (ttyfd != -1)
        {
            dup2(ttyfd, STDIN_FILENO);
            close(ttyfd);
        }
        return (true);
    }
    
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
    
    if (!ft_strncmp(line, "exit", 4) && (line[4] == '\0' || line[4] == ' '))
    {
        free(line);
        return (false);
    }
    
    if (init(root))
        return (false);
    
    add_history(line);
    handle_signals();  // Switch to execution signal handling
    process_command(root->data, line);
    cleanup_after_command(root->data, &line);
    
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
	root.data = &data;
	data.root = &root;
	root.env = set_env_var_list(&root, envp);
	if (initialize_shell(&root, prompt, sizeof(prompt)))
		return (1);
	while (execute_command_loop(&root, prompt))
		;
	exit_status = root.last_exit_status;
	cleanup(&data, &root);
	ft_lstclear(&root.list_of_list, NULL);
	my_free(&root.malloc_root);
	return (exit_status);
}