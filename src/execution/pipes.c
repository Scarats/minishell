#include "../minishell.h"

// Will call the left node.
int left(t_node *node, t_main_data *data)
{
	close(node->pipefd[0]);
	data->in_child = true;
	traverse_tree(node->left, data);
	exit(0);
}

// Will call the right node.
int right(t_node *node, t_main_data *data)
{
	close(node->pipefd[1]);
	data->in_child = true;
	traverse_tree(node->right, data);
	exit(0);
}

// Will create two childs, left and right, for each end of the pipe.
int pipes(t_node *node, t_main_data *data)
{
	if (pipe(node->pipefd) == -1)
		return (perror("pipe"), 1);
	
	node->left->input_fd = node->input_fd;
    node->left->output_fd = node->pipefd[1];
    node->right->input_fd = node->pipefd[0];
    node->right->output_fd = node->output_fd;

	node->left_pid = fork();
	if (node->left_pid == -1 )
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
	if (waitpid(node->left_pid, NULL, 0) == -1 || waitpid(node->right_pid, NULL, 0) == -1)
		return (1);
	return (0);
}
