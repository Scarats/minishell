/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:36:31 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/30 22:54:47 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Call step by step each function for clean execution.
int	exec_handler(t_main_data *data, t_node *node)
{
	int	error;

	error = 0;
	error = set_io_fds(node, data);
	if (error != 0)
		return (error);
	error = redirections(node, data);
	if (error != 0)
		return (error);
	if (!node->cmd_argv || !node->cmd_argv[0])
		return (0);
	if (node->builtin)
		return (execution(node, data));
	if (get_bin_path(node, data) != 0)
		return (127);
	return (execution(node, data));
}

/* void	handle_child(t_main_data *data, t_node *node)
{
	int		error;
	t_env	*path;
	t_root	*root;

	root = data->root;
	error = exec_handler(data, node);
	if (error)
	{
		path = find_tenv_var(root->env, "PATH");
		if (error == 127 && (!path || !path->value))
			fdprintf(2, "minishell: %s: No such file or directory\n",
				node->cmd_argv[0]);
		else
			print_exec_error(error, node);
	}
	my_multi_free(&root->list_of_list);
	exit(error);
} */

void	handle_child(t_main_data *data, t_node *node)
{
    int		error;
    t_env	*path;
    t_root	*root;

    // Reset child signals to defaults so Ctrl-C / Ctrl-\ affect the child like bash
    struct sigaction sa_dfl;
    sa_dfl.sa_handler = SIG_DFL;
    sigemptyset(&sa_dfl.sa_mask);
    sa_dfl.sa_flags = 0;
    sigaction(SIGINT, &sa_dfl, NULL);
    sigaction(SIGQUIT, &sa_dfl, NULL);

    root = data->root;
    error = exec_handler(data, node);
    if (error)
    {
        path = find_tenv_var(root->env, "PATH");
        if (error == 127 && (!path || !path->value))
            fdprintf(2, "minishell: %s: No such file or directory\n",
                node->cmd_argv[0]);
        else
            print_exec_error(error, node);
    }
    my_multi_free(&root->list_of_list);
    exit(error);
}

// Handle the execution process.
// Should handle the bin before creating and opening the files.
/* int	exec_cmd(t_node *node, t_main_data *data)
{
	int	pid;
	int	status;

	if (node->builtin)
		return (if_builtin(node, data));
	status = 0;
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
		handle_child(data, node);
	if (node->input_fd != -1 && node->input_fd != STDIN_FILENO)
		close(node->input_fd);
	if (node->output_fd != -1 && node->output_fd != STDOUT_FILENO)
		close(node->output_fd);
	if (waitpid(pid, &status, 0) == -1)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
} */

/* int	exec_cmd(t_node *node, t_main_data *data)
{
	int		pid;
	int		status;
	int		error;
	//t_env	*path;
	t_root	*root;

	root = data->root;
    if (node->builtin)
    {
        if (data->in_child)
            return exec_handler(data, node);
        return exec_builtin_in_parent(node, data);
    }
    pid = fork();
    if (pid == -1)
        return 1;
    if (pid == 0)
    {
        signal(SIGQUIT, SIG_DFL);
        error = exec_handler(data, node);
        if (error == 127 && node->cmd_argv && node->cmd_argv[0])
            fdprintf(2, "minishell: %s: command not found\n", node->cmd_argv[0]);
        my_multi_free(&root->list_of_list);
        exit(error);
    }
    if (node->input_fd != -1 && node->input_fd != STDIN_FILENO)
        close(node->input_fd);
    if (node->output_fd != -1 && node->output_fd != STDOUT_FILENO)
        close(node->output_fd);

    if (waitpid(pid, &status, 0) == -1)
    {
        if (WIFEXITED(status))
            root->last_exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            root->last_exit_status = 128 + WTERMSIG(status);
    }
    handle_signals();
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return 1;
	if (node->builtin)
	{
		if (data->in_child)
			return (exec_handler(data, node));
		return (exec_builtin_in_parent(node, data));
	}
	return (1);
} */

int	exec_cmd(t_node *node, t_main_data *data)
{
    int				pid;
    int				status;
    t_root			*root;
    struct sigaction	old_int;
    struct sigaction	old_quit;
    struct sigaction	ign;

    root = data->root;

    // Builtins: run in parent if not in a child context
    if (node->builtin)
    {
        if (data->in_child)
            return exec_handler(data, node);
        return exec_builtin_in_parent(node, data);
    }

    pid = fork();
    if (pid == -1)
        return 1;
    if (pid == 0)
    {
        // Child: default signals, then run full exec handler and exit with its code
        handle_child(data, node); // never returns
    }

    // Parent: close any fds we duplicated on the node
    if (node->input_fd != -1 && node->input_fd != STDIN_FILENO)
        close(node->input_fd);
    if (node->output_fd != -1 && node->output_fd != STDOUT_FILENO)
        close(node->output_fd);

    // Temporarily ignore SIGINT/SIGQUIT while waiting (avoid extra newline, EINTR)
    ign.sa_handler = SIG_IGN;
    sigemptyset(&ign.sa_mask);
    ign.sa_flags = 0;
    sigaction(SIGINT, &ign, &old_int);
    sigaction(SIGQUIT, &ign, &old_quit);

    // Wait, handling EINTR properly
    while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
        ;

    // Restore shell signal handlers
    sigaction(SIGINT, &old_int, NULL);
    sigaction(SIGQUIT, &old_quit, NULL);

    // Map status like bash (130 for SIGINT, 131 for SIGQUIT), and print Quit message
    set_last_exit_status_from_wait(root, status);
    return root->last_exit_status;
}
