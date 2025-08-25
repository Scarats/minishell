#include "../../minishell.h"

// Return the index of the most significant operator.
// If none found, return -1;
int find_operator(t_tokenizer tok_list, int size)
{

}

// make a recursive function that will find the most significant operator.
// create a node from it.
// Call itself from the first token of the left part, and call itself from the first token of the right part.
// Continue until no operator is found.
int build_tree(t_main_data *data, t_node *node, t_token *tok_list, int size)
{
	int i;

	i = find_operator(tok_list, size);
	if (i < 0)
	{
		// end of recursion.
	}
	else
	{
		// create new_node at tok_list[i]
		// Left
		build_tree(data, new_node, tok_list[0], size - i);
		build_tree(data, new_node, tok_list[i], size - i);
	}

}