#include "../minishell.h"

int or_or(t_node *node, t_main_data *data)
{
	if (traverse_tree(node->left, data) != 0)
		return (traverse_tree(node->right, data), 0);
	return (1);
}
