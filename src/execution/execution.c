/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:44:54 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/29 21:43:25 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Execute the command.
// Should not return since the program will be replaced by execve.
int	execution(t_node *node, t_main_data *data)
{
	t_env		*path;
	struct stat	st;
	t_root		*root;

	root = data->root;
	if (!data || !node)
		return (1);
	if (node->builtin)
		return (exec_builtins(node, data));
	path = find_tenv_var(root->env, "PATH");
	if ((!path || !path->value) && !strchr(node->cmd_argv[0], '/'))
		return (127);
	if (stat(node->cmd_argv[0], &st) == 0 && S_ISDIR(st.st_mode))
	{
		errno = EISDIR;
		return (126);
	}
	if (ft_strchr(node->cmd_argv[0], '/'))
		execve(node->cmd_argv[0], node->cmd_argv, t_env_to_char_arr(root,
				root->env));
	else if (!node->path)
		return (127);
	return (execve(node->path, node->cmd_argv, t_env_to_char_arr(root,
				root->env)));
}

void	subshell_child(t_main_data *data, t_node *node)
{
	int		error;
	t_root	*root;

	root = data->root;
	error = 0;
	node->create_subshell = false;
	error = traverse_tree(node, data);
	my_multi_free(&root->list_of_list);
	exit(error);
}

int	create_subshell(t_node *node, t_main_data *data)
{
	int		pid;
	int		status;
	t_root	*root;

	root = data->root;
	status = 0;
	pid = -1;
	pid = fork();
	if (pid < 0)
		return (1);
	else if (pid == 0)
		subshell_child(data, node);
	if (waitpid(pid, &status, 0) == -1)
	{
		if (WIFEXITED(status))
			root->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			root->last_exit_status = 128 + WTERMSIG(status);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

// Will check the type of the node.
// Then redirect to the according type function.
// These functions will execute recursively by calling traverse_tree again.
// The recursion breakpoint is reached when a NODE_COMMAND is reached.
int	traverse_tree(t_node *node, t_main_data *data)
{
	if (!node || !data)
		return (1);
	else if (node->create_subshell)
		return (create_subshell(node, data));
	else if (node->type == NODE_COMMAND)
		return (exec_cmd(node, data));
	else if (node->type == NODE_PIPE)
		return (pipes(node, data));
	else if (node->type == NODE_AND)
		return (and_and(node, data));
	else if (node->type == NODE_OR)
		return (or_or(node, data));
	return (1);
}
