/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:09:55 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/22 14:09:57 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	find_builtin(int *error, t_node *node, int length, t_main_data *data)
{
	if (!ft_strncmp(node->cmd_argv[0], "cd", length))
		error = cd(data->root, node);
	else if (!ft_strncmp(node->cmd_argv[0], "pwd", length))
		error = pwd();
	else if (!ft_strncmp(node->cmd_argv[0], "echo", length))
		error = echo(&node->cmd_argv[1]);
	else if (!ft_strncmp(node->cmd_argv[0], "matrix", length))
		error = matrix(node->cmd_argv);
	else if (!ft_strncmp(node->cmd_argv[0], "export", length))
		error = export(data->root, node->cmd_argv);
	else if (!ft_strncmp(node->cmd_argv[0], "env", length))
		error = env(data->root->env);
	else if (!ft_strncmp(node->cmd_argv[0], "unset", length))
		error = unset(data->root, &data->root->env, &node->cmd_argv[1]);
	else if (!ft_strncmp(node->cmd_argv[0], "exit", length))
		error = exit_builtin(node, data);
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
