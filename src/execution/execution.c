#include "../../minishell.h"

// Will check the type of the node.
// Then redirect to the according type.
// It will execute recursively.
int traverse_tree(t_node *node ,t_main_data *data)
{
	if (data->node->type == NODE_COMMAND)
		return (exec_cmd());
	else if (data->node->type == NODE_PIPE)
		return (pipex(node, data));
	else if (node->type == NODE_AND)
		return (and_and(node, data));
	else if (node->type == NODE_OR)
		return (or_or(node, data));
	return (1); // Should not arrive here i guess
}
