#include "../minishell.h"

int add_file(t_main_data *data, t_node *node, t_token *token)
{
	if (token->type != TOKEN_FILE)
		return (1);
	else if (node->redirection == TOKEN_REDIRECT_OUT)
	{
		node->output = ft_strdup(token->word);
		my_addtolist(data->malloc_tree, node->output);
		return (0);
	}
	else if (node->redirection == TOKEN_REDIRECT_IN)
	{
		node->input = ft_strdup(token->word);
		my_addtolist(data->malloc_tree, node->input);
		return (0);
	}
	return (1);
}

int create_node_cmd(t_main_data *data, t_token *tok_list, t_node *node, int size)
{
	int i;
	int y;
	int cmd_toks;

	cmd_toks = 0;
	while (tok_list[cmd_toks].type == TOKEN_CMD || tok_list[cmd_toks].type == TOKEN_ARGUMENT)
		cmd_toks++;
	node->cmd = my_malloc(data->malloc_tree, sizeof(t_cmd));
	i = -1;
	while (++i < size)
	{

		else if (cmd_toks > 0)
		{
			y = 0;
			node->cmd->tokens = my_malloc(data->malloc_tree, sizeof(char *) * cmd_toks);
			while (y < cmd_toks)
			{
				node->cmd->tokens[y] = ft_strdup(tok_list[y].word);
				my_addtolist(data->malloc_tree, node->cmd->tokens[y]);
				y++;
			}
			node->cmd->tokens[cmd_toks] = '\0';
		}
		else if (tok_list[i].type == TOKEN_REDIRECT_IN || TOKEN_REDIRECT_OUT || TOKEN_APPEND || TOKEN_HEREDOC)
			node->redirection = tok_list[i].type;
		else if (tok_list[i].type == TOKEN_FILE)
			add_file(data, node, &tok_list[i]);
	}
	return (0);
}

// While size, add token, check its type and add it to the node.
t_node *create_node(t_main_data *data, t_token *tok_list, t_node_type type,
					int size)
{
	t_node *node;

	node = my_malloc(data->malloc_tree, sizeof(t_node));
	ft_memset(node, 0, sizeof(t_node));
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
	// make 2 funcions
	// pos = find_and_or())
	// if (pos)
	// 	return (pos);
	// return (find_pipe());
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
		node = create_node(data, tok_list, map_tok_to_node(tok_list[0].type), size);
		return (node);
	}
	else
	{
		// Create the new node in the call of each side.
		node = create_node(data, tok_list, map_token_to_node(tok_list[i].type), size);

		node->left = build_tree(data, tok_list, i);
		node->right = build_tree(data, tok_list + i + 1, size - (i + 1));
		return (node);
	}
}
