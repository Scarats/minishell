#include "../minishell.h"

// Check if the token is a redirection: 1 yes 0 no.
int	is_redir(t_token_type t)
{
	return (t == TOKEN_REDIRECT_IN || t == TOKEN_REDIRECT_OUT || t == TOKEN_APPEND || t == TOKEN_HEREDOC);
}

// Check if && or ||
int	is_and_or(t_token_type t)
{
    return (t == TOKEN_AND_AND || t == TOKEN_OR);
}

int	is_word_token(t_token_type t)
{
    return (t == TOKEN_CMD || t == TOKEN_ARGUMENT || t == TOKEN_FILE
        || t == TOKEN_ENV_VAR || t == TOKEN_TEXT);
}

// Check the conditions of what comes before and after '('
// returns 1 for error and 0 for no error.
// Prev token can be an operator or ).
int check_left_par(t_token *tok_list, int index, int size)
{
	t_token_type prev;
	if (index > 0)
    {
		prev = tok_list[index - 1].type;
        if (!is_and_or(prev) && prev != TOKEN_LPAREN)
            return (1);
    }
    if (index + 1 >= size)
        return (1);
    if (!is_word_token(tok_list[index + 1].type) && tok_list[index + 1].type != TOKEN_LPAREN)
        return (1);
    return (0);
}

// Check the conditions of what comes before and after ')'
// returns 1 for error and 0 for no error.
// Prev token can be a word or ).
int check_right_par(t_token *tok_list, int index, int size)
{
	t_token_type prev;
	if (index > 0)
    {
		// Previous token word or )
		prev = tok_list[index - 1].type;
		if (!is_word_token(&tok_list[-1].type) && tok_list[-1].type != TOKEN_RPAREN)
			return (1);
	}
	// Next token operator or ) or end-of-input
	else if (!is_and_or(&tok_list[1].type) && tok_list[1].type != TOKEN_RPAREN || index == size)
		return (1);
	return (0);

}

// Check for (a)(b), (((a) && b errors.
int check_paren_error(t_token *tok_list, int size)
{
	int i;
	int depth;
	bool left;
	bool right;

	left = false;
	right = false;
	i = -1;
	depth = 0;
	while (++i < size)
	{
		if (depth < 0)
			return (1);
		else if (tok_list[i].type == TOKEN_LPAREN && !check_left_par(tok_list[i].type, i, size)) // Check that is's not the last char
		{
			depth++;
			if (i != 0 && (tok_list[i - 1].type == TOKEN_AND_AND || tok_list[i - 1].type == TOKEN_OR))
				left = !left;
		}
		// Check last param
		else if (tok_list[i].type == TOKEN_RPAREN && !check_right_par(tok_list[i].type, i, size))
		{
			depth--;
		}
	}

	if (depth != 0)
		return (1); // Error.
}

// Check all the tokens are in parenthesis (tokens).
int wrapped_in_parren(t_token *tok_list, int size)
{
	int i;
	int depth;

	i = -1;
	depth = 0;
	if (size < 2 || !tok_list[0].type == TOKEN_LPAREN || !tok_list[size - 1].type == TOKEN_RPAREN) 
		return (0);
	// What happens if size == 2 and it's just () ?
	if (size == 2)
		return (-1);
	while (++i < size - 1)
	{
		if (tok_list[i].type == TOKEN_LPAREN)
			depth++;
		else if (tok_list[i].type == TOKEN_RPAREN)
			depth--;
		if (depth == 0 && i != size - 2)
			return (0);
	}
	return (depth == 1); // Depth should be at 1 since we stopped before the last one.
}


// Return the number of cmd and args in tok_list
int get_cmd_argc(t_token *tok_list, int size)
{
	int i;
	int cmd_argc;

	i = 0;
	cmd_argc = 0;
	while (i < size)
	{
		if (tok_list[i].type == TOKEN_CMD || tok_list[i].type == TOKEN_ARGUMENT)
			cmd_argc++;
		i++;
	}
	return (cmd_argc);
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

// Create new node to the list, malloc and memset it, add its type.
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
