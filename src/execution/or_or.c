#include "../minishell.h"

int or_or(t_node *node, t_main_data *data)
{
	int error;

	error = traverse_tree(node->left, data);
	if (error)
		return (traverse_tree(node->right, data));
	return (error);
}
