/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:36:31 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/17 19:26:51 by tcardair         ###   ########.fr       */
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

void	handle_child(t_main_data *data, t_node *node)
{
    int		error;
    t_env	*path;
    t_root	*root;

    root = data->root;
    if (node->pipefd[0] >= 0)
        close(node->pipefd[0]);
    if (node->pipefd[1] >= 0)
        close(node->pipefd[1]);
    node->pipefd[0] = -1;
    node->pipefd[1] = -1;
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
int	exec_cmd(t_node *node, t_main_data *data)
{
	int	pid;
	int	status;

	if (node->builtin && !node->in_pipe)
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
}
