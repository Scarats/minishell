/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:06:50 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/14 14:48:02 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_stop_flag = 0;

static void	sigint_prompt(int sig)
{
    (void)sig;
    g_stop_flag = 1;
    // async-signal-safe
    write(STDOUT_FILENO, "\n", 1);
}

static void	sigint_heredoc(int sig)
{
    (void)sig;
    g_stop_flag = 1;
    // async-signal-safe
    write(STDOUT_FILENO, "\n", 1);
    // Tell readline loop to finish as soon as it returns to main thread
    rl_done = 1;
}

void	handle_signals(void)
{
    struct sigaction	sa;

    ft_bzero(&sa, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigint_prompt;
    sigaction(SIGINT, &sa, NULL);

    // Ignore SIGQUIT at the prompt
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void	handle_signals_heredoc(void)
{
    struct sigaction	sa;

    ft_bzero(&sa, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigint_heredoc;
    sigaction(SIGINT, &sa, NULL);

    // Ignore SIGQUIT during heredoc
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void	handle_signals_execution_parent(void)
{
    // While waiting for children, parent ignores Ctrl-C and Ctrl
    //signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

static int	process_command(t_main_data *data, char *line)
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
    {
        // Parent ignores signals while executing; children must restore defaults before execve
        handle_signals_execution_parent();
        root->last_exit_status = traverse_tree(data->node, data);
        // Back to interactive mode for next prompt
        handle_signals();
    }
    else
        return (1);
    return (root->last_exit_status);
}

static bool	execute_command_loop(t_root *root, char *prompt)
{
    char *line;

    // Ensure interactive signal policy before each readline
    handle_signals();
    line = readline(prompt);

    // If SIGINT happened during readline, g_stop_flag is set by handler
    if (g_stop_flag)
    {
        root->last_exit_status = 130;
        g_stop_flag = 0;

        if (line)
            free(line);

        // Do readline UI cleanup/redraw in the main thread (safe)
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
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
    process_command(root->data, line);
    cleanup_after_command(root->data, &line);
    return (true);
}

int	main(int ac, char **av, char **envp)
{
    t_main_data data;
    t_root root;
    char prompt[1024];
    int exit_status;

    ft_memset(&root, 0, sizeof(root));
    ft_memset(&data, 0, sizeof(data));
    (void)ac;
    (void)av;
    exit_status = 0;
    root.data = &data;
    data.root = &root;
    root.env = set_env_var_list(&root, envp);

    // Let us fully own signal handling (readline won’t install its own)
    rl_catch_signals = 0;

    handle_signals();
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