#include "../../minishell.h"

// Will call the left node.
int left(t_node *node)
{
	close(node->pipefd[0]);
	if (node->pipefd[1] != STDOUT_FILENO)
		dup2(node->pipefd[1], STDOUT_FILENO);
	close(node->pipefd[1]);
	traverse_tree(node->left);
	exit(0);
}

// Will call the right node.
int right(t_node *node)
{
	close(node->pipefd[1]);
	if (node->pipefd[1] != STDOUT_FILENO)
		dup2(node->pipefd[1], STDOUT_FILENO);
	close(node->pipefd[0]);
	traverse_tree(node->right);
	exit(0);
}

// Will create two childs, left and right, for each end of the pipe.
int pipex(t_node *node)
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
		left(node);

	node->right_pid = fork();
	if (node->right_pid == -1)
		return (perror("fork"), 1);
	else if (node->right_pid == 0)
		right(node);

	close(node->pipefd[0]);
	close(node->pipefd[1]);
	waitpid(node->left_pid, NULL, 0);
	waitpid(node->right_pid, NULL, 0);
}
