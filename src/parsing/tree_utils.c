#include "../minishell.h"

int is_builtin(char *cmd)
{
	int i;
	int size;
    const char *builtins[] = {
        "echo",
        "cd",
        "pwd",
        "export",
        "unset",
        "env",
        "exit",
        NULL
    };

	i = 0;
	size = ft_strlen(cmd);
	if (size < 1)
		return (0);
	while(builtins[i])
	{
		if (size == ft_strlen(builtins[i]) && ft_strncmp(builtins[i], cmd, (size_t)size) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	is_op_or_redir(t_token_type t)
{
    return (t == TOKEN_PIPE || t == TOKEN_AND_AND || t == TOKEN_OR
        || t == TOKEN_REDIRECT_IN || t == TOKEN_REDIRECT_OUT
        || t == TOKEN_APPEND || t == TOKEN_HEREDOC);
}

int	is_redir(t_token_type t)
{
    return (t == TOKEN_REDIRECT_IN || t == TOKEN_REDIRECT_OUT
        || t == TOKEN_APPEND || t == TOKEN_HEREDOC);
}

// Check if && or ||
int is_and_or(t_token_type t)
{
	return (t == TOKEN_AND_AND || t == TOKEN_OR);
}

int is_word_token(t_token_type t)
{
	return (t == TOKEN_CMD || t == TOKEN_ARGUMENT || t == TOKEN_FILE || t == TOKEN_ENV_VAR || t == TOKEN_TEXT);
}

// Check the conditions of what comes before and after '('
// returns 1 for error and 0 for no error.
// Prev token can be an operator or (.
int check_left_par(t_token *tok_array, int index, int size)
{
	t_token_type prev;

	if (index > 0)
	{
		prev = tok_array[index - 1].type;
		// Allow preceding: ( , any binary op, or redirection
		if (!is_op_or_redir(prev) && prev != TOKEN_LPAREN)
			return (1);
	}
	if (index + 1 >= size)
		return (1);
	else if (!is_word_token(tok_array[index + 1].type) && tok_array[index + 1].type != TOKEN_LPAREN)
		return (1);
	return (0);
}

// Check the conditions of what comes before and after ')'
// returns 1 for error and 0 for no error.
// Prev token can be a word or ).
int check_right_par(t_token *tok_array, int index, int size)
{
	t_token_type prev;

	if (index == 0)
		return (1); // Cannot start with )
	else if (index > 0)
	{
		// Previous token word or )
		prev = tok_array[index - 1].type;
		if (!is_word_token(prev) && prev != TOKEN_RPAREN)
			return (1);
	}
	if (index + 1 >= size)
		return (0); // end-of-input after ')'
					// Next token: allow any binary op, redirection, or another ')'
	if (!is_op_or_redir(tok_array[index + 1].type) && tok_array[index + 1].type != TOKEN_RPAREN)
		return (1);
	return (0);
}

// Check for (a)(b), (((a) && b errors.
int check_paren_error(t_token *tok_array, int size)
{
	int i;
	int depth;

	i = -1;
	depth = 0;
	while (++i < size)
	{
		if (depth < 0)
			return (1);
		else if (tok_array[i].type == TOKEN_LPAREN)
		{
			if (check_left_par(tok_array, i, size))
				return (1);
			depth++;
		}
		else if (tok_array[i].type == TOKEN_RPAREN)
		{
			if (check_right_par(tok_array, i, size))
				return (1);
			depth--;
		}
	}
	if (depth != 0)
		return (1);
	return (0);
}

// Check all the tokens are in parenthesis (tokens).
int wrapped_in_paren(t_token *tok_array, int size)
{
	int i;
	int depth;

	i = -1;
	depth = 0;
	if (size < 2 || tok_array[0].type != TOKEN_LPAREN || tok_array[size - 1].type != TOKEN_RPAREN)
		return (0);
	// What happens if size == 2 and it's just () ?
	if (size == 2)
		return (0); // Error
	while (++i < size - 1)
	{
		if (tok_array[i].type == TOKEN_LPAREN)
			depth++;
		else if (tok_array[i].type == TOKEN_RPAREN)
			depth--;
		if (depth == 0 && i != size - 2)
			return (0);
	}
	return (depth == 1); // Depth should be at 1 since we stopped before the last one.
}

// Return the number of cmd and args in tok_array
int get_cmd_argc(t_token *tok_array, int size)
{
	int i;
	int cmd_argc;

	i = 0;
	cmd_argc = 0;
	while (i < size)
	{
		if (tok_array[i].type == TOKEN_CMD || tok_array[i].type == TOKEN_ARGUMENT)
			cmd_argc++;
		i++;
	}
	return (cmd_argc);
}

// take token type and return node type.
t_node_type map_token_to_node(t_token_type t)
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
t_redir *add_redirection(t_main_data *data, t_node *node, t_token_type type)
{
	t_redir *redirection;
	t_redir *last;

	redirection = my_malloc(&data->malloc_tree, sizeof(t_redir));
	redirection->type = type;
	redirection->fd = -1;
	if (!node->redirection)
		node->redirection = redirection;
	else
	{
		last = node->redirection;
		while (last->next)
			last = last->next;
		last->next = redirection;
	}
	return (redirection);
}
