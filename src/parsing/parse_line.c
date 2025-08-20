#include "../../minishell.h"

// Determine if the word is a command, argument, filename etc...
// It will be done according to the previous tokens created.
t_token_type get_word_type(t_main_data *data)
{
	t_token_type prev_type;

	// It's the first node, then COMMAND
	if (data->tok->last_token->prev_token == NULL)
		return (TOKEN_CMD);
	prev_type = data->tok->last_token->prev_token->type;
	if (prev_type == TOKEN_REDIRECT_OUT || prev_type == TOKEN_REDIRECT_IN || prev_type == TOKEN_APPEND || prev_type == TOKEN_HEREDOC)
		return (TOKEN_FILE);
	else if (prev_type == TOKEN_PIPE || prev_type == TOKEN_AND_AND || prev_type == TOKEN_OR || prev_type == TOKEN_LPAREN || prev_type == TOKEN_RPAREN)
		return (TOKEN_CMD);
	else if (prev_type == TOKEN_CMD || prev_type == TOKEN_ARGUMENT)
		return (TOKEN_ARGUMENT);
	else if (prev_type == TOKEN_DOLLAR)
		return (TOKEN_ENV_VAR);
	else
		return (TOKEN_ERROR);
}

// Create token, add them to the list and add type.
int create_token(t_main_data *data, int start, int end, t_token_type type)
{
	t_token *lst;

	if (type == TOKEN_SPACE)
		return (0);
	lst = add_to_list(data, data->tok->last_token, start, end);
	if (!lst)
		return (1);
	if (type == TOKEN_TEXT)
		type = get_word_type(data);
	lst->type = type;
	lst->word = clean_string(lst->word);
	return (0);
}

t_char_type get_char_type(char c)
{
	if (c == '\0')
		return (CHAR_NULL);
	else if (c == ' ')
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

t_token_type get_tok_type(char c, char next)
{
	if (c == '\0')
		return (TOKEN_NULL);
	else if (c == '&' && next == '&')
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

int tokenizer(t_main_data *data)
{
	t_token_type tok_type;

	tok_type = TOKEN_NULL;
	while (data->tok->pos < data->tok->length)
	{
		data->tok->curr_char_type = get_char_type(data->tok->input[data->tok->pos]);
		handle_quotes(data);
		// For operators, always create a token when the character type changes OR when we have consecutive operators
		if (data->tok->curr_char_type != data->tok->prev_char_type)
		{
			if (data->tok->prev_char_type != CHAR_SPACE)
			{
				// Create with previous_char_type
				// Can be done in one line lol
				tok_type = get_tok_type(data->tok->input[data->tok->prev_pos], check_next_char(data->tok->input, data->tok->prev_pos));
				create_token(data, data->tok->prev_pos, data->tok->pos, tok_type);
			}
			data->tok->prev_pos = data->tok->pos;
		}
		data->tok->prev_char_type = data->tok->curr_char_type;
		data->tok->pos++;
	}
	// Handle the final token if needed
	if (data->tok->prev_char_type != CHAR_SPACE && data->tok->prev_pos < data->tok->pos)
	{
		tok_type = get_tok_type(data->tok->input[data->tok->prev_pos], check_next_char(data->tok->input, data->tok->prev_pos));
		create_token(data, data->tok->prev_pos, data->tok->pos, tok_type);
	}
	return (0);
}

// Parse the input.
// Tokenize it, then create a binary tree.
int parser(t_main_data *data)
{
	if (tokenizer(data))
		return (1);
	// if (build_tree(data))
	// return (1);
	return (0);
}
