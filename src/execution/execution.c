#include "../minishell.h"

// Will call the left node.
int left(t_node *node)
{
	close(node->pipefd[0]); // close unused read
	dup2(node->pipefd[1], STDOUT_FILENO);
	close(node->pipefd[1]);
	// Setup the next node fds
	execute_node(node->left, node->input_fd, STDOUT_FILENO);
	exit(0);
}

// Will call the right node.
int right(t_node *node)
{
	close(node->pipefd[1]); // close unused read
	dup2(node->pipefd[0], STDIN_FILENO);
	close(node->pipefd[0]);
	// Setup the next node fds
	node->right->input->fd = STDIN_FILENO;
	execute_node(node->right, node->output_fd, STDOUT_FILENO);
	exit(0);
}

// Will create two childs, left and right, for each end of the pipe.
int pipex(t_node *node)
{
	if (pipe(node->pipefd) == -1)
		return (perror("pipe"), 1);

	node->left_pid = fork();
	node->right_pid = fork();
	if (node->left_pid == -1 || node->right_pid == -1)
		return (perror("fork"), 1);
	if (node->left_pid == 0)
		left(&node);
	if (node->right_pid == 0)
		right(&node);
	close(node->pipefd[0]);
	close(node->pipefd[1]);
	waitpid(node->left_pid, NULL, 0);
	waitpid(node->right_pid, NULL, 0);
}

// Will check the type of the node.
// Then redirect to the according type.
// It will execute recursively.
//
// TO FIGURE OUT:
// When to switch from left to right node.
int execute_node(t_node *node)
{
	// 1. Check type.
	if (node->type == NODE_COMMAND)
		// Leaf node
		// Exec
		// return.
		if (node->type == NODE_PIPE)
			//
			if (node->type == NODE_AND)
				if (node->type == NODE_OR)
	//
}