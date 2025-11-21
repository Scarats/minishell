/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:55:44 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/21 16:27:12 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int wait_child(pid_t pid)
{
    int	status;

    if (pid <= 0)
        return (0);
    while (waitpid(pid, &status, 0) == -1)
    {
        if (errno != EINTR)
            return (1);
    }
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return (1);
}

// Will call the left node.
int	left(t_node *node, t_main_data *data)
{
	int	error;
	int	saved_write;

	error = 0;
	saved_write = node->pipefd[1];
	node->pipe_left = 1;
	node->in_pipe = true;
	node->left->in_pipe = true;
	node->left->pipe_left = 1;
	node->left->pipefd[0] = node->pipefd[0];
	node->left->pipefd[1] = node->pipefd[1];
	node->left->output_fd = node->pipefd[1];
	if (node->input_fd != -1)
		node->left->input_fd = node->input_fd;
	error = traverse_tree(node->left, data);
	if (saved_write >= 0)
		close(saved_write);
	node->left->pipefd[0] = -1;
	node->left->pipefd[1] = -1;
	return (error);
}

// Will call the right node.
int	right(t_node *node, t_main_data *data)
{
	int	error;
	int	saved_read;

	error = 0;
	saved_read = node->pipefd[0];
	node->pipe_right = 1;
	node->in_pipe = true;
	node->right->in_pipe = true;
	node->right->pipe_right = 1;
	node->right->pipefd[0] = node->pipefd[0];
	node->right->pipefd[1] = node->pipefd[1];
	node->right->input_fd = node->pipefd[0];
	if (node->output_fd != -1)
		node->right->output_fd = node->output_fd;
	error = traverse_tree(node->right, data);
	if (saved_read >= 0)
		close(saved_read);
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
    int	error;
    int	tmp;
    int status;

    if (!node || !data)
        return (1);
    if (pipe(node->pipefd) == -1)
        return (perror("pipe"), 1);
    error = left(node, data);
    tmp = right(node, data);
    if (tmp != 0)
        error = tmp;
    error = wait_child(node->left_pid);
    error = wait_child(node->right_pid);
    /* reap any remaining child processes so their output (e.g. valgrind
       summaries) is printed before the shell prompt */
    while (waitpid(-1, &status, 0) > 0)
        ;
    close_pipe_pair(node->pipefd);
    node->pipefd[0] = -1;
    node->pipefd[1] = -1;
    return (error);
}
