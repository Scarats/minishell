/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:06:50 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/06 16:29:32 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_stop_flag = 0;

/* void	handle_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
} */

/* void	handler(int sig)
{
	if (sig == SIGINT)
	{
		g_stop_flag = 1;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		//rl_redisplay();
		//rl_done = 1;
	}
	else if (sig == SIGQUIT)
	{
		write(STDOUT_FILENO, "minishell: quit (core dumped)\n", 31);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
} */
/* void	handler(int sig)
{
    if (sig == SIGINT)
    {
        g_stop_flag = 1;
        write(STDOUT_FILENO, "\n", 1);
        // Do NOT call rl_on_new_line/rl_replace_line/rl_redisplay here.
        // Only tell readline to stop and return control.
        rl_done = 1;
    }
} */

int	process_command(t_main_data *data, char *line)
{
	t_root	*root;

	root = data->root;
	reset_tokenizer_for_line(data->tok, line);
	if (g_stop_flag)
	{
		root->last_exit_status = 130;
		g_stop_flag = 0;
		return (130);
	}
	if (!parser(data))
		root->last_exit_status = traverse_tree(data->node, data);
	else
	{
		if(!g_stop_flag)
			root->last_exit_status = 2;
		return (1);
	}
	return (root->last_exit_status);
}

/* bool	execute_command_loop(t_root *root, char *prompt) first function (working)
{
	char	*line;

	line = NULL;
	fflush(stdout);
	if (g_stop_flag)
	{
		root->last_exit_status = 130;
		g_stop_flag = 0;		
		rl_done = 0;
		//rl_on_new_line();
		return (true);
	}
	rl_done = 0;
	line = readline(prompt);
	if (rl_done)
	{
		rl_done = 0;
		return (false);
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
	if (init(root))
		return (false);
	add_history(line);
	process_command(root->data, line);
	cleanup_after_command(root->data, &line);
	return (true);
} */

bool	execute_command_loop(t_root *root, char *prompt)
{
    char	*line;

    line = NULL;
    if (g_stop_flag)
    {
        root->last_exit_status = 130;
        g_stop_flag = 0;
        //rl_done = 0;  // Reset for the next readline
		//rl_on_new_line();
        return (true);  // Restart the loop in main()
    }
    //rl_done = 0;
    line = readline(prompt);
    if (g_stop_flag)
    {
        g_stop_flag = 0;
        root->last_exit_status = 130;
        if (line)
            free(line);
        //rl_done = 0;
        //rl_on_new_line();
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
    if (init(root))
        return (false);
    add_history(line);
    process_command(root->data, line);
    cleanup_after_command(root->data, &line);
	if (g_stop_flag)
    {
       	root->last_exit_status = 130;
        g_stop_flag = 0;

		int	ttyfd;

		ttyfd = open("/dev/tty", O_RDONLY);
        if (ttyfd != -1)
        {
            dup2(ttyfd, STDIN_FILENO);
            close(ttyfd);
        }
    }
    return (true);
}

/* bool	execute_command_loop(t_root *root, char *prompt)
{
    char	*line;

    line = NULL;
    fflush(stdout);

    while (1)
    {
        rl_done = 0;                 // start a fresh readline session
        line = readline(prompt);

        // If SIGINT interrupted readline, clean up readline state here (not in handler),
        // set exit status, and restart the prompt without eating the next key.
        if (g_stop_flag)
        {
            root->last_exit_status = 130;
            g_stop_flag = 0;

            if (line)
                free(line);
            line = NULL;

            // Now safely reset readline UI state
			rl_replace_line("", 0);
            rl_on_new_line();            
            //rl_redisplay();

            // Show the prompt again by looping
            continue;
        }
        break;
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
    if (init(root))
        return (false);
    add_history(line);
    process_command(root->data, line);
    cleanup_after_command(root->data, &line);
    return (true);
} */

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