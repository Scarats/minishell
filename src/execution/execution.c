#include "../minishell.h"

int left(t_node *node)
{

}

int right(t_node *node)
{

}

// Will create two childs, left and right, for each end of the pipe.
int	pipex(t_node *node)
{
	int pipefd[2];
	pid_t	left_pid;	
	pid_t	right_pid;	

	if (pipe(pipefd) == -1)
		return (perror("pipe"), 1);

	left_pid = fork();
	right_pid = fork();
	if (left_pid == -1 || right_pid == -1)
		return (perror("fork"), 1);
	if (left_pid == 0)
		left(&node);
	if (right_pid == 0) 
		right(&node);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
}


// Will check the type of the node.
// Then redirect to the according type.
// It will execute recursively.
//
// TO FIGURE OUT:
// When to switch from left to right node.
int execute_node(t_node	*node)
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