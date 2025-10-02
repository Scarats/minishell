#include "../minishell.h"

// remove_token: unlink a token from a doubly-linked list
void	remove_token(t_token **head, t_token *t)
{
	if (!t)
		return ;
	if (t->prev_token)
		t->prev_token->next_token = t->next_token;
	else
		*head = t->next_token;
	if (t->next_token)
		t->next_token->prev_token = t->prev_token;
}

// Determine if the word is a command, argument, filename etc...
// It will be done according to the previous tokens created.
t_token_type	get_word_type(t_token *tok)
{
	t_token_type	prev_type;

	// It's the first node, then COMMAND
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

// Create token, add them to the list and add type.
int	create_token(t_main_data *data, int start, int end, t_token_type type)
{
	t_token		*tok;
	char		*slice;
	char		*expanded;
	static int	i;

	if (!i)
		i = 1;
	if (type == TOKEN_SPACE)
		type = TOKEN_TEXT;
	tok = add_to_list(data, data->tok->last_token);
	if (!tok)
		return (1);
	if (type == TOKEN_TEXT)
		type = get_word_type(tok);
	tok->type = type;
	// Always grab the raw lexeme (operators, parens, words, etc.)
	slice = ft_substr(data->tok->input, start, end - start);
	if (slice)
		my_addtolist(&data->malloc_tok, slice);
	tok->word = slice;
	// Handle environment variable expansion (but NOT inside single quotes)
	if (tok->type == TOKEN_ENV_VAR)
	{
		expanded = get_env_var(data->root->env, slice);
		if (tok->prev_token && tok->prev_token->type == TOKEN_DOLLAR)
			remove_token(&data->tok->token_list, tok->prev_token);
		tok->type = get_word_type(tok); // Recompute final role (CMD/ARG/FILE)
		if (expanded)
			tok->word = expanded;
		// Use expanded value (do not track if from env)
	}
	printf(RED "tok %i = %s\n" RESET, i++, tok->word);
	// tok->word = clean_string(tok->word);
	return (0);
}

t_char_type	get_char_type(char c)
{
	if (c == ' ')
		return (CHAR_SPACE);
	else if (c == '|' || c == '&' || c == '<' || c == '>' || c == '$')
		return (CHAR_OPERATOR);
	else if (c == '(' || c == ')')
		return (CHAR_PARENTHESIS);
	else if (c == '\'')
		return (CHAR_SINGLE_QUOTE);
	else if (c == '"')
		return (CHAR_DOUBLE_QUOTE);
	return (CHAR_TEXT);
}

t_token_type	get_tok_type(char c, char next)
{
	if (c == '&' && next == '&')
		return (TOKEN_AND_AND);
	else if (c == '|' && next == '|')
		return (TOKEN_OR);
	else if (c == '<' && next == '<')
		return (TOKEN_HEREDOC);
	else if (c == '>' && next == '>')
		return (TOKEN_APPEND);
	else if (c == '$')
		return (TOKEN_DOLLAR);
	else if (c == '&')
		return (TOKEN_AND);
	else if (c == '|')
		return (TOKEN_PIPE);
	else if (c == '<')
		return (TOKEN_REDIRECT_IN);
	else if (c == '>')
		return (TOKEN_REDIRECT_OUT);
	else if (c == '(')
		return (TOKEN_LPAREN);
	else if (c == ')')
		return (TOKEN_RPAREN);
	else if (c == ' ')
		return (TOKEN_SPACE);
	return (TOKEN_TEXT);
}

int	handle_operator(t_main_data *data, t_token_type *tok_type)
{
	int	len;

	len = 0;
	if (!data || !tok_type)
		return (1);
	*tok_type = get_tok_type(data->tok->input[data->tok->prev_pos],
			check_next_char(data->tok->input, data->tok->prev_pos));
	if (*tok_type == TOKEN_AND_AND || *tok_type == TOKEN_OR
		|| *tok_type == TOKEN_HEREDOC || *tok_type == TOKEN_APPEND)
		len = 2;
	else
		len = 1;
	create_token(data, data->tok->prev_pos, data->tok->prev_pos + len,
		*tok_type);
	data->tok->prev_pos += len;
	return (0);
}

int	handle_normal_token(t_main_data *data, t_token_type *tok_type)
{
	if (!data || !tok_type)
		return (1);
	if (data->tok->prev_char_type != CHAR_SPACE
		&& data->tok->prev_pos < data->tok->pos)
	{
		*tok_type = get_tok_type(data->tok->input[data->tok->prev_pos],
				check_next_char(data->tok->input, data->tok->prev_pos));
		create_token(data, data->tok->prev_pos, data->tok->pos, *tok_type);
	}
	data->tok->prev_pos = data->tok->pos;
	return (0);
}

int	handle_parenthesis(t_main_data *data, t_token_type *tok_type)
{
	if (!data || !tok_type)
		return (1);
	*tok_type = get_tok_type(data->tok->input[data->tok->prev_pos],
			check_next_char(data->tok->input, data->tok->prev_pos));
	create_token(data, data->tok->prev_pos, data->tok->pos, *tok_type);
	data->tok->prev_pos = data->tok->pos;
	return (0);
}

int	tokenizer(t_main_data *data)
{
	t_token_type	tok_type;

	tok_type = TOKEN_NULL;
	while (data->tok->pos < data->tok->length)
	{
		data->tok->curr_char_type = get_char_type(data->tok->input[data->tok->pos]);
		handle_quotes(data->tok, data);
		if (data->tok->curr_char_type == CHAR_PARENTHESIS
			&& data->tok->prev_char_type == CHAR_PARENTHESIS
			&& data->tok->pos > data->tok->prev_pos)
			handle_parenthesis(data, &tok_type);
		else if (data->tok->curr_char_type == CHAR_OPERATOR
			&& data->tok->prev_char_type == CHAR_OPERATOR
			&& data->tok->pos > data->tok->prev_pos)
			handle_operator(data, &tok_type);
		else if (data->tok->curr_char_type != data->tok->prev_char_type)
			handle_normal_token(data, &tok_type);
		data->tok->prev_char_type = data->tok->curr_char_type;
		data->tok->pos++;
	}
	if (data->tok->prev_char_type != CHAR_SPACE
		&& data->tok->prev_pos < data->tok->pos
		&& data->tok->prev_pos < data->tok->length && !data->tok->single_quote)
		/* prevent empty trailing token */
	{
		tok_type = get_tok_type(data->tok->input[data->tok->prev_pos],
				check_next_char(data->tok->input, data->tok->prev_pos));
		create_token(data, data->tok->prev_pos, data->tok->pos, tok_type);
	}
	/* New: detect unclosed quotes */
	if (data->tok->double_quote || data->tok->single_quote)
		return (fdprintf(2, "minishell: syntax error: unclosed quote\n"), 1);
	return (0);
}

// Turn the token linked list in an array, easier for AST.
int	list_to_array(t_main_data *data, t_token *token_list, int size)
{
	t_token	*curr_tok;
	int		i;
	int		real_size;

	// Recompute size after possible removals (e.g., '$' tokens)
	real_size = 0;
	curr_tok = token_list;
	while (curr_tok)
	{
		real_size++;
		curr_tok = curr_tok->next_token;
	}
	size = real_size;
	data->tok->token_list_size = real_size;
	i = 0;
	data->tok->token_array = my_malloc(&data->root->list_of_list,
			&data->malloc_tok, sizeof(t_token) * (size > 0 ? size : 1));
	if (!data->tok->token_array && size > 0)
		return (1);
	curr_tok = token_list;
	while (curr_tok && i < size)
	{
		data->tok->token_array[i] = *curr_tok;
		i++;
		curr_tok = curr_tok->next_token;
	}
	return (0);
}

int	check_op_syntax(t_token *tok)
{
	if (!tok)
		return (1);
	printf(PURPLE "\nWORD = %s\n", tok->word);
	if (tok->type == TOKEN_AND_AND && ft_strncmp(tok->word, "&&", 2))
		return (printf(GREEN "\nCASE &&\n" RESET), 1);
	else if (tok->type == TOKEN_OR && ft_strncmp(tok->word, "||", 2))
		return (printf(GREEN "\nCASE ||\n" RESET), 1);
	printf(GREEN "\nELSE\n" RESET);
	return (0);
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

int	syntax_check(t_token *token_array, int size)
{
	int				i;
	t_token_type	t;

	if (!token_array || size <= 0)
		return (1);
	if (size == 1 && !(is_redir(token_array[0].type)
			|| is_word_token(token_array[0].type)))
		return (syntax_error(token_array[0].word), 1);
	i = 0;
	while (i < size)
	{
		t = token_array[i].type;
		if (is_operator(t))
		{
			if (i == 0 || !is_command_end(token_array[i - 1].type))
				return (syntax_error(token_array[i].word), 1);
			if (i + 1 >= size)
				return (syntax_error("newline"), 1);
			if (!is_command_start(token_array[i + 1].type))
				return (syntax_error(token_array[i + 1].word), 1);
		}
		else if (is_redir(t))
		{
			if (i + 1 >= size)
				return (syntax_error("newline"), 1);
			if (!is_word_token(token_array[i + 1].type))
				return (syntax_error(token_array[i + 1].word), 1);
			i++;
		}
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
