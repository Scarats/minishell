/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:55:44 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/17 18:52:33 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Will call the left node.
int	left(t_node *node, t_main_data *data)
{
    int	error;

    error = 0;
    node->pipe_left = 1;
    node->left->output_fd = node->pipefd[1];
    node->pipefd[1] = -1;
    if (node->input_fd != -1)
        node->left->input_fd = node->input_fd;
    error = traverse_tree(node->left, data);
    return (error);
}

// Will call the right node.
int	right(t_node *node, t_main_data *data)
{
    int	error;

    error = 0;
    node->pipe_right = 1;
    node->right->input_fd = node->pipefd[0];
    node->pipefd[0] = -1;
    if (node->output_fd != -1)
        node->right->output_fd = node->output_fd;
    error = traverse_tree(node->right, data);
    return (error);
}

int	pipes_logic(t_node *node, t_main_data *data)
{
	int error;

	error = 0;
	if (pipe(node->pipefd) == -1)
		return (perror("pipe"), 1);
	// node->left_pid = fork();
	// if (node->left_pid == -1)
	// 	return (perror("fork"), 1);
	// else if (node->left_pid == 0)
	error = left(node, data);
	// node->right_pid = fork();
	// if (node->right_pid == -1)
	// 	return (perror("fork"), 1);
	// else if (node->right_pid == 0)
	error = right(node, data);
	return (error);
}

// Will create two childs, left and right, for each end of the pipe.
int	pipes(t_node *node, t_main_data *data)
{
    int	error;

    error = 0;
    if (!node || !data)
        return (1);
    if (pipe(node->pipefd) == -1)
        return (perror("pipe"), 1);
    error = left(node, data);
    if (error == 0)
        error = right(node, data);
    if (node->pipefd[0] != -1)
        close(node->pipefd[0]);
    if (node->pipefd[1] != -1)
        close(node->pipefd[1]);
    node->pipefd[0] = -1;
    node->pipefd[1] = -1;
    return (error);
}
