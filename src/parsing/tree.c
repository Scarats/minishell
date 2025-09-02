#include "../minishell.h"

int add_cmd_argv(t_main_data *data, t_node *node, int cmd_argv, t_token *tok_list)
{
	int y;

	y = -1;
	node->argv_cmd = my_malloc(data->malloc_tree, sizeof(char *) * (cmd_argv + 1));
	while (++y < cmd_argv)
	{
		node->argv_cmd[y] = ft_strdup(tok_list[y].word);
		my_addtolist(data->malloc_tree, node->argv_cmd[y]);
	}
	node->argv_cmd[cmd_argv] = NULL;
	return (0);
}

int add_file(t_main_data *data, t_redir *curr_redir, t_token *token)
{
	if (token->type != TOKEN_FILE || !curr_redir)
		return (1);
	curr_redir->filename = ft_strdup(token->word);
	my_addtolist(data->malloc_tree, curr_redir->filename);
	return (0);
}

int create_node_cmd(t_main_data *data, t_token *tok_list, t_node *node, int size)
{
	int i;
	int cmd_argv;
	t_redir *curr_redir;

	curr_redir = NULL;
	cmd_argv = 0;
	while (tok_list[cmd_argv].type == TOKEN_CMD || tok_list[cmd_argv].type == TOKEN_ARGUMENT)
		cmd_argv++;
	if (cmd_argv > 0)
		add_cmd_argv(data, node, cmd_argv, tok_list);
	// Redirection handling
	i = -1;
	while (++i < size)
	{
		if (is_redir(tok_list[i].type))
			curr_redir = add_redirection(data, node, tok_list[i].type);
		else if (tok_list[i].type == TOKEN_FILE)
			add_file(data, curr_redir, &tok_list[i]); // Add the file to curr_redir
	}
	return (0);
}

// While size, add token, check its type and add it to the node.
t_node *create_node(t_main_data *data, t_token *tok_list, t_node_type type,
					int size)
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
	// make 2 functions
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
		node = create_node(data, tok_list, map_tok_to_node(tok_list[0].type),
						   size);
		return (node);
	}
	else
	{
		// Create the new node in the call of each side.
		node = create_node(data, tok_list, map_token_to_node(tok_list[i].type),
						   size);
		node->left = build_tree(data, tok_list, i);
		node->right = build_tree(data, tok_list + i + 1, size - (i + 1));
		return (node);
	}
}
