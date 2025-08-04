#include "../minishell.h"

// Will check the type of the node.
// Then redirect to the according type.
// It will execute recursively.
int execute_node(t_node	*node, int input_fd, int output_fd)
{
	// 1. Check type.
	if (node->type == NODE_COMMAND)
		// Exec
		// return.
	if (node->type == NODE_PIPE)
	if (node->type == NODE_AND)
	if (node->type == NODE_OR)
	// 		
	// 2. Call type function.
}