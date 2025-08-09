#include "../../minishell.h"

int tokenizer()
{
}

// Parse the input.
// Tokenize it, then create a binary tree.
int parser(t_main_data *data)
{
	if (tokenizer(data))
		return (1);
	if (build_tree(data))
		return (1);
	return (0);
}
