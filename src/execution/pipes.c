/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:55:44 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/12 13:56:45 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Will call the left node.
int	left(t_node *node, t_main_data *data)
{
	int		error;
	t_root	*root;

	root = data->root;
	error = 0;
	close(node->pipefd[0]);
	if (dup2(node->pipefd[1], STDOUT_FILENO) == -1)
		exit(1);
	close(node->pipefd[1]);
	if (node->input_fd != -1)
		node->left->input_fd = node->input_fd;
	data->in_child = true;
	error = traverse_tree(node->left, data);
	my_multi_free(&root->list_of_list);
	exit(error);
}

// Will call the right node.
int	right(t_node *node, t_main_data *data)
{
	int		error;
	t_root	*root;

	root = data->root;
	error = 0;
	close(node->pipefd[1]);
	if (dup2(node->pipefd[0], STDIN_FILENO) == -1)
		exit(1);
	close(node->pipefd[0]);
	if (node->output_fd != -1)
		node->right->output_fd = node->output_fd;
	data->in_child = true;
	error = traverse_tree(node->right, data);
	my_multi_free(&root->list_of_list);
	exit(error);
}

int	pipes_logic(t_node *node, t_main_data *data)
{
	if (pipe(node->pipefd) == -1)
		return (perror("pipe"), 1);
	node->left_pid = fork();
	if (node->left_pid == -1)
		return (perror("fork"), 1);
	else if (node->left_pid == 0)
		left(node, data);
	node->right_pid = fork();
	if (node->right_pid == -1)
		return (perror("fork"), 1);
	else if (node->right_pid == 0)
		right(node, data);
	close(node->pipefd[0]);
	close(node->pipefd[1]);
	return (0);
}

// Will create two childs, left and right, for each end of the pipe.
int	pipes(t_node *node, t_main_data *data)
{
	int		status_left;
	int		status_right;
	t_root	*root;

	if (!node || !data)
		return (1);
	root = data->root;
	if (pipes_logic(node, data))
		return (1);
	if (waitpid(node->left_pid, &status_left, 0) == -1
		|| waitpid(node->right_pid, &status_right, 0) == -1)
		return (1);
	if (WIFEXITED(status_right))
	{
		root->last_exit_status = WEXITSTATUS(status_right);
		return (root->last_exit_status);
	}
	if (WIFSIGNALED(status_right))
	{
		root->last_exit_status = 128 + WTERMSIG(status_right);
		return (root->last_exit_status);
	}
	root->last_exit_status = 0;
	return (0);
}
