#include "../minishell.h"



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
	if (node->type == NODE_PIPE)
		pipex(node);
	if (node->type == NODE_AND)
	if (node->type == NODE_OR)
	//
}