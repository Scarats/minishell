/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:55:44 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/17 23:17:39 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Will call the left node.
int	left(t_node *node, t_main_data *data)
{
    int	error;

    error = 0;
    node->pipe_left = 1;
    node->in_pipe = true;
    node->left->in_pipe = true;
    node->left->pipefd[0] = node->pipefd[0];
    node->left->pipefd[1] = -1;
    node->left->output_fd = node->pipefd[1];
    if (node->input_fd != -1)
        node->left->input_fd = node->input_fd;
    error = traverse_tree(node->left, data);
    if (node->left->output_fd != -1)
    {
        close(node->left->output_fd);
        node->left->output_fd = -1;
    }
    node->left->pipefd[0] = -1;
    node->left->pipefd[1] = -1;
    return (error);
}

// Will call the right node.
int	right(t_node *node, t_main_data *data)
{
    int	error;

    error = 0;
    node->pipe_right = 1;
    node->in_pipe = true;
    node->right->in_pipe = true;
    node->right->pipefd[1] = node->pipefd[1];
    node->right->pipefd[0] = -1;
    node->right->input_fd = node->pipefd[0];
    if (node->output_fd != -1)
        node->right->output_fd = node->output_fd;
    error = traverse_tree(node->right, data);
    if (node->right->input_fd != -1)
    {
        close(node->right->input_fd);
        node->right->input_fd = -1;
    }
    node->right->pipefd[0] = -1;
    node->right->pipefd[1] = -1;
    return (error);
}

void	close_pipe_pair(int fd[2])
{
    if (fd[0] >= 0)
        close(fd[0]);
    if (fd[1] >= 0)
        close(fd[1]);
}

// Will create two childs, left and right, for each end of the pipe.
int	pipes(t_node *node, t_main_data *data)
{
    int error;

    if (!node || !data)
        return (1);
    if (pipe(node->pipefd) == -1)
        return (perror("pipe"), 1);
    error = left(node, data);
    error = right(node, data);
    close_pipe_pair(node->pipefd);
    node->pipefd[0] = -1;
    node->pipefd[1] = -1;
    return (error);
}
