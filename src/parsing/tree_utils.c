#include "../minishell.h"

int	is_redir(t_token_type t)
{
	return (t == TOKEN_REDIRECT_IN || t == TOKEN_REDIRECT_OUT || t == TOKEN_APPEND || t == TOKEN_HEREDOC);
}

// take token type and return node type.
t_node_type	map_token_to_node(t_token_type t)
{
	if (t == TOKEN_PIPE)
		return (NODE_PIPE);
	if (t == TOKEN_AND_AND)
		return (NODE_AND);
	if (t == TOKEN_OR)
		return (NODE_OR);
	return (NODE_COMMAND);
}

// Create new node to the list, malloc and memset it.
// If first, initialize the list.
t_redir	*add_redirection(t_main_data *data, t_node *node, t_token_type type)
{
	t_redir *redirection;
	t_redir *last;

	redirection = my_malloc(data->malloc_tree, sizeof(t_redir));
	if (!node->redirection)
		node->redirection = redirection;
	else
	{
		last = node->redirection;
		while (last->next != NULL)
			last = last->next;
		last->next = redirection;
	}
	redirection->type = type;
	return (redirection);
}
