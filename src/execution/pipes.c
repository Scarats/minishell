#include "../minishell.h"

// Will call the left node.
int	left(t_node *node, t_main_data *data)
{
	int	error;

	error = 0;
	close(node->pipefd[0]);
	// Make pipe the default stdout in this child first
	if (dup2(node->pipefd[1], STDOUT_FILENO) == -1)
		exit(1);
	close(node->pipefd[1]);
	// Pass parent's input fd into the left subtree so set_io_fds can use it
	if (node->input_fd != -1)
		node->left->input_fd = node->input_fd;
	// ensure not to pre-set left->output_fd here; pipe is already set via dup2
	data->in_child = true;
	error = traverse_tree(node->left, data);
	my_multi_free(&data->root->list_of_list);
	exit(error);
}

// Will call the right node.
int	right(t_node *node, t_main_data *data)
{
	int	error;

	error = 0;
	close(node->pipefd[1]);
	// Make pipe the default stdin in this child first
	if (dup2(node->pipefd[0], STDIN_FILENO) == -1)
		exit(1);
	close(node->pipefd[0]);
	// Pass parent's output fd into the right subtree so set_io_fds can use it
	if (node->output_fd != -1)
		node->right->output_fd = node->output_fd;
	// ensure not to pre-set right->input_fd here; pipe is already set via dup2
	data->in_child = true;
	error = traverse_tree(node->right, data);
	my_multi_free(&data->root->list_of_list);
	exit(error);
}

// Will create two childs, left and right, for each end of the pipe.
int	pipes(t_node *node, t_main_data *data)
{
	int	status_left;
	int	status_right;

	if (!node || !data)
		return (1);
	if (pipe(node->pipefd) == -1)
		return (perror("pipe"), 1);
	printf(RED "PIPE\n" RESET);
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
	if (waitpid(node->left_pid, &status_left, 0) == -1
		|| waitpid(node->right_pid, &status_right, 0) == -1)
		return (1);
	if (WIFEXITED(status_right))
	{
		data->root->last_exit_status = WEXITSTATUS(status_right);
		return (data->root->last_exit_status);
	}
	if (WIFSIGNALED(status_right))
	{
		data->root->last_exit_status = 128 + WTERMSIG(status_right);
		return (data->root->last_exit_status);
	}
	data->root->last_exit_status = 0;
	return (0);
}
