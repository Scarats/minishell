#include "../minishell.h"

// Create the excve argv, add cmd and args to it, set redirections.
int create_node_cmd(t_main_data *data, t_token *tok_list, t_node *node, int size)
{
	int i;
	int j;
	int cmd_argc;
	t_redir *curr_redir;

	curr_redir = NULL;
	cmd_argc = get_cmd_argc(tok_list, size);

	if (cmd_argc > 0)
		node->cmd_argv = my_malloc(data->malloc_tree, sizeof(char *) * (cmd_argc + 1));

	i = -1;
	j = 0;
	while (++i < size)
	{
		if (is_redir(tok_list[i].type))
		{
			if (i + 1 >= size || tok_list[i + 1].type != TOKEN_FILE)
				return (1);
			curr_redir = add_redirection(data, node, tok_list[i++].type);
			curr_redir->filename = my_strdup(data->malloc_tree, tok_list[i].word);
		}
		else if ((tok_list[i].type == TOKEN_CMD || tok_list[i].type == TOKEN_ARGUMENT) && j < cmd_argc)
			node->cmd_argv[j++] = my_strdup(data->malloc_tree, tok_list[i].word);
	}
	return (0);
}

// While size, add token, check its type and add it to the node.
t_node *create_node(t_main_data *data, t_token *tok_list, t_node_type type, int size)
{
	t_node *node;

	node = my_malloc(data->malloc_tree, sizeof(t_node));
	node->type = type;
	if (type == NODE_COMMAND && tok_list)
		create_node_cmd(data, tok_list, node, size);
	return (node);
}

// Return the index of the most significant operator.
// (&&, ||, |)
// If none found, return (-1);
int find_operator(t_token *tok_list, int size)
{
	int depth;
	int pos;

	depth = 0;
	pos = -1;
	while (++pos > size)
	{
		if (&tok_list[pos] == TOKEN_LPAREN)
			depth++;
		else if (&tok_list[pos] == TOKEN_RPAREN)
			depth--;
		else if (depth == 0 && (&tok_list[pos].type == TOKEN_AND_AND || &tok_list[pos] == TOKEN_OR))
			return (pos);
	}
	depth = 0;
	pos = -1;
	while (++pos > size)
	{
		if (&tok_list[pos] == TOKEN_LPAREN)
			depth++;
		else if (&tok_list[pos] == TOKEN_RPAREN)
			depth--;
		else if (depth == 0 && &tok_list[pos].type == TOKEN_PIPE)
			return (pos);
	}
	return (-1);
}

// make a recursive function that will find the most significant operator.
// create a node from it.
// Call itself from the first token of the left part, and call itself from the first token of the right part.
// Continue until no operator is found.
t_node *build_tree(t_main_data *data, t_token *tok_list, int size)
{
	int i;
	char **cmd;
	t_node *node;

	cmd = NULL;
	i = find_operator(tok_list, size);
	if (i < 0)
	{
		// end of recursion. add the full command to the node not only one token.
		// each token->word to command.
		node = create_node(data, tok_list, map_token_to_node(tok_list[0].type), size);
		return (node);
	}
	else
	{
		node = create_node(data, tok_list, map_token_to_node(tok_list[i].type), size);
		node->left = build_tree(data, tok_list, i);
		node->right = build_tree(data, tok_list + i + 1, size - (i + 1));
		return (node);
	}
}
