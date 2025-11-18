/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:09:55 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/18 22:29:02 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Run builtin in parent: save fds, apply redirs, run, then restore.
int	exec_builtin_in_parent(t_node *node, t_main_data *data)
{
	t_fd_backup	fd;
	int			error;

	if (node->cmd_argv[0] && ft_strcmp(node->cmd_argv[0], "exit") == 0)
		return (exec_handler(data, node));
	backup_fds(&fd);
	data->fd_backup = fd;
	error = exec_handler(data, node);
	reset_fds(&fd);
	return (error);
}

void	find_builtin(int *error, t_node *node, int length, t_main_data *data)
{
	t_root	*root;

	root = data->root;
	if (!ft_strncmp(node->cmd_argv[0], "cd", length))
		*error = cd(root, node);
	else if (!ft_strncmp(node->cmd_argv[0], "pwd", length))
		*error = pwd();
	else if (!ft_strncmp(node->cmd_argv[0], "echo", length))
		*error = echo(&node->cmd_argv[1]);
	else if (!ft_strncmp(node->cmd_argv[0], "matrix", length))
		*error = matrix(node->cmd_argv);
	else if (!ft_strncmp(node->cmd_argv[0], "export", length))
		*error = export(root, node->cmd_argv);
	else if (!ft_strncmp(node->cmd_argv[0], "env", length))
		*error = env(root->env);
	else if (!ft_strncmp(node->cmd_argv[0], "unset", length))
		*error = unset(root, &root->env, &node->cmd_argv[1]);
	else if (!ft_strncmp(node->cmd_argv[0], "exit", length))
		*error = exit_builtin(node, data);
	else
		printf(RED "error: built-in not found.\n" RESET);
}

// Execute built-in functions.
int	exec_builtins(t_node *node, t_main_data *data)
{
	int	error;
	int	length;

	if (!data)
		data = NULL;
	if (!node)
		return (1);
	length = ft_strlen(node->cmd_argv[0]);
	error = 0;
	find_builtin(&error, node, length, data);
	return (error);
}
