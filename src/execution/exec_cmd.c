/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:36:31 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/21 17:35:15 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_unused_pipe_end(t_node *node)
{
	if (node->pipe_left && node->pipefd[0] >= 0)
		close(node->pipefd[0]);
	if (node->pipe_right && node->pipefd[1] >= 0)
		close(node->pipefd[1]);
}

void	close_ancestor_pipes(t_node *node)
{
	while (node)
	{
		close_pipe_pair(node->pipefd);
		node->pipefd[0] = -1;
		node->pipefd[1] = -1;
		node = node->parent;
	}
}

// Call step by step each function for clean execution.
int	exec_handler(t_main_data *data, t_node *node)
{
	int	error;

	error = set_io_fds(node, data);
	if (error != 0)
		return (error);
	error = redirections(node, data);
	if (error != 0)
		return (error);
	close_ancestor_pipes(node->parent);
	if (!node->cmd_argv || !node->cmd_argv[0])
		return (0);
	if (node->builtin)
		return (execution(node, data));
	if (get_bin_path(node, data) != 0)
		return (127);
	return (execution(node, data));
}

void	handle_child(t_main_data *data, t_node *node)
{
	int		error;
	t_env	*path;
	t_root	*root;

	root = data->root;
	error = exec_handler(data, node);
	if (node->pipefd[0] >= 0)
		close(node->pipefd[0]);
	if (node->pipefd[1] >= 0)
		close(node->pipefd[1]);
	if (error == 127)
	{
		path = find_tenv_var(root->env, "PATH");
		if (!path || !path->value)
			fdprintf(2, "minishell: %s: No such file or directory\n",
				node->cmd_argv[0]);
		else
			print_exec_error(error, node);
	}
	else if (error && error != 130)
		print_exec_error(error, node);
	my_multi_free(&root->list_of_list);
	exit(error);
}

// Handle the execution process.
// Should handle the bin before creating and opening the files.
int	exec_cmd(t_node *node, t_main_data *data)
{
	int	pid;
	int	status;

	if (node->builtin && !node->in_pipe)
		return (exec_builtin_in_parent(node, data));
	status = 0;
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close_unused_pipe_end(node);
		handle_child(data, node);
	}
	assign_pid(pid, node);
	if (!node->in_pipe)
		close_unused_pipe_end(node);
	cleanup_parent_fds(node);
	if (node->in_pipe)
		return (0);
	return (wait_for_child(pid));
}
