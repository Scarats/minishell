#include "../../minishell.h"

// Will check the type of the node.
// Then redirect to the according type.
// It will execute recursively.
int traverse_tree(t_node *node)
{
	// 1. Check type.
	if (node->type == NODE_COMMAND)
	// Leaf node
	// Exec
	// return.
	else if (node->type == NODE_PIPE)
		pipex(node);
	// else if (node->type == NODE_AND)
	// else if (node->type == NODE_OR)
	//
}
