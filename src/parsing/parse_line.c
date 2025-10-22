#include "../minishell.h"

// Determine if the word is a command, argument, filename etc...
// It will be done according to the previous tokens created.
t_token_type	get_word_type(t_token *tok)
{
	t_token_type	prev_type;

	if (tok->prev_token == NULL)
		return (TOKEN_CMD);
	prev_type = tok->prev_token->type;
	if (prev_type == TOKEN_REDIRECT_OUT || prev_type == TOKEN_REDIRECT_IN
		|| prev_type == TOKEN_APPEND || prev_type == TOKEN_HEREDOC)
		return (TOKEN_FILE);
	else if (prev_type == TOKEN_PIPE || prev_type == TOKEN_AND_AND
		|| prev_type == TOKEN_OR || prev_type == TOKEN_LPAREN
		|| prev_type == TOKEN_RPAREN)
		return (TOKEN_CMD);
	else if (prev_type == TOKEN_DOLLAR)
		return (TOKEN_ENV_VAR);
	else
		return (TOKEN_ARGUMENT);
}

// A command can start with a word-like token or a left parenthesis
int	is_command_start(t_token_type t)
{
	return (is_word_token(t) || t == TOKEN_LPAREN || t == TOKEN_REDIRECT_OUT);
}

// A command can end with a word-like token or a right parenthesis
int	is_command_end(t_token_type t)
{
	return (is_word_token(t) || t == TOKEN_RPAREN);
}

int	syntax_check_logic(int *i, t_token *token_array, int size)
{
	t_token_type	t;

	t = token_array[*i].type;
	if (is_operator(t))
	{
		if (*i == 0 || !is_command_end(token_array[*i - 1].type))
			return (syntax_error(token_array[*i].word), 1);
		if (*i + 1 >= size)
			return (syntax_error("newline"), 1);
		if (!is_command_start(token_array[*i + 1].type))
			return (syntax_error(token_array[*i + 1].word), 1);
	}
	else if (is_redir(t))
	{
		if (*i + 1 >= size)
			return (syntax_error("newline"), 1);
		if (!is_word_token(token_array[*i + 1].type))
			return (syntax_error(token_array[*i + 1].word), 1);
		*i += 1;
	}
	return (0);
}

int	syntax_check(t_token *token_array, int size)
{
	int	i;
	int error;

	if (!token_array || size <= 0)
		return (1);
	if (size == 1 && !((token_array[0].type)
			|| is_word_token(token_array[0].type)))
		return (syntax_error(token_array[0].word), 1);
	else if (size == 1 && ft_strlen(token_array[0].word) == 1
		&& token_array[0].word[0] == '$')
		return (fdprintf(2, "minishell: $: command not found\n"), 1);
	i = 0;
	error = 0;
	while (i < size)
	{
		error = syntax_check_logic(&i, token_array, size);
		if (error)
			return (error);
		i++;
	}
	return (0);
}

// Parse the input.
// Tokenize it, then create a binary tree.
int	parser(t_main_data *data)
{
	if (tokenizer(data))
		return (1);
	if (list_to_array(data, data->tok->token_list, data->tok->token_list_size))
		return (1);
	if (syntax_check(data->tok->token_array, data->tok->token_list_size))
		return (1);
	data->node = build_tree(data, data->tok->token_array,
			data->tok->token_list_size, 0);
	if (!data->node)
		return (1);
	return (0);
}
