#include "../../minishell.h"

// Will check the type of the node.
// Then redirect to the according type.
// It will execute recursively.
int traverse_tree(t_node *node ,t_main_data *data)
{
	// 1. Check type.
	if (data->node->type == NODE_COMMAND)
	// Leaf node
	// Exec
	// return.
	if (data->node->type == NODE_PIPE && pipex(node, data) != 0)
		return (1); // Call error function.
	// else if (node->type == NODE_AND)
	// else if (node->type == NODE_OR)
	//
	return (0);
}
