#include "../minishell.h"

int and_and(t_node *node, t_main_data *data)
{
	int error;

	error = traverse_tree(node->left, data);
	if (error == 0)
		return (traverse_tree(node->right, data));
	return (error);
}