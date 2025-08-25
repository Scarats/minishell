#include "../../minishell.h"

t_node *create_node(t_node_type type, char **words)
{
	t_node node;

	ft_memset(&node, 0, sizeof(node));
	node.type = type;
	if (type == NODE_COMMAND)
		node.cmd->tokens = words;
	return (&node);
}

// Return the index of the most significant operator.
// If none found, return -1;
int find_operator(t_token *tok_list, int size)
{

}

// make a recursive function that will find the most significant operator.
// create a node from it.
// Call itself from the first token of the left part, and call itself from the first token of the right part.
// Continue until no operator is found.
int build_tree(t_main_data *data, t_node *node, t_token *tok_list, int size)
{
	int i;
	char **cmd;

	cmd = NULL;
	i = find_operator(tok_list, size);
	if (i < 0)
	{
		// end of recursion. add the full command to the node not only one token.
		// each token->word to command.
		cmd = my_malloc(data->malloc_tree, sizeof(char *) * size);
		while (++i < size)
			cmd[i] = ft_strdup(tok_list[i].word);
		my_array_addtolist(data->malloc_tree, cmd);
		node = create_node(tok_list[0].type, cmd);
		return (0);
	}
	else
	{
		node = create_node(tok_list[i].type, tok_list[i].word);
		// create new_node at tok_list[i]
		// Left
		if (build_tree(data, node->left, &tok_list[0], i) || build_tree(data, node->right, &tok_list[i], size - i))
			return (1);
		return (0);
	}
}
