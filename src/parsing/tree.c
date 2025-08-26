#include "../../minishell.h"

int set_file(t_main_data *data, t_node *node, t_token *token)
{
	if (token->type != TOKEN_FILE)
		return (0);
	else if (token->type == TOKEN_REDIRECT_OUT)
	{
		node->output = ft_strdup(token->word);
		my_addtolist(data->malloc_tree, node->output);
	}
	else if (token->type == TOKEN_REDIRECT_IN)
	{
		node->input = ft_strdup(token->word);
		my_addtolist(data->malloc_tree, node->input);
	}
	return (0);
}

int create_node_cmd(t_main_data *data, t_token *tok_list, t_node *node, int size)
{
	int i;
	int cmd_toks;
	
	i = -1;
	cmd_toks = 0;
	while (++i < size)
	{
		if (&tok_list[i].type == TOKEN_CMD || tok_list[i].type == TOKEN_ARGUMENT)
			cmd_toks++;
		else
		{
			if (cmd_toks > 0)
			{
				node->cmd->tokens = my_malloc(data->malloc_tree, sizeof(char *) * cmd_toks);
				while (cmd_toks > 0)
				{
					node->cmd->tokens[cmd_toks--] = ft_strdup(&tok_list[cmd_toks].word);
					my_addtolist(data->malloc_tree, node->cmd->tokens[cmd_toks]);
				}
			}	
		}
		if (&tok_list[i].type == TOKEN_REDIRECT_IN || TOKEN_REDIRECT_OUT || TOKEN_APPEND || TOKEN_HEREDOC)
			node->redirection = &tok_list[i].type;
		set_file(data, node, &tok_list[i]); 
	}
}


// While size, add token, check its type and add it to the node.
t_node *create_node(t_main_data *data, t_token *tok_list, t_node_type type, int size)
{
	t_node *node;

	node = my_malloc(data->malloc_tree, sizeof(t_node));
	ft_memset(&node, 0, sizeof(node));
	node->type = type;
	if (type != NODE_COMMAND)
		return (&node);
	create_node_cmd(data, tok_list, node, size);
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
		node = create_node(tok_list[0].type, cmd, size);
		return (0);
	}
	else
	{
		node = create_node(tok_list[i].type, tok_list[i].word, size);
		// create new_node at tok_list[i]
		// Left
		if (build_tree(data, node->left, &tok_list[0], i) || build_tree(data, node->right, &tok_list[i], size - i))
			return (1);
		return (0);
	}
}
