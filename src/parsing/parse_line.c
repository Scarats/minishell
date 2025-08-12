#include "../../minishell.h"

// Determine if the word is a command, argument, filename etc...
// It will be done according to the previous tokens created.
t_token_type get_word_type(t_main_data *data)
{
	t_token_type prev_type;

	// It's the first node, then COMMAND
	if (data->tok->last_token->prev_token == NULL)
		return (TOKEN_CMD);
	prev_type = data->tok->last_token->type;
	if (prev_type == TOKEN_REDIRECT_OUT || prev_type == TOKEN_REDIRECT_IN || prev_type == TOKEN_APPEND || prev_type == TOKEN_HEREDOC)
		return (TOKEN_FILE);
	else if (prev_type == TOKEN_PIPE || prev_type == TOKEN_AND_AND || prev_type == TOKEN_OR)
		return (TOKEN_CMD);
	else if (prev_type == TOKEN_CMD || prev_type == TOKEN_SPARAM)
		return (TOKEN_ARGUMENT);
	else
		return(TOKEN_ERROR);
}

int create_token(t_main_data *data, int start, int end, t_token_type type)
{
	t_token *lst;

	lst = add_to_list(data, data->tok->last_token, start, end);
	if(!lst)
		return(1);
	if (type == TOKEN_TEXT)
		type = get_word_type(data);
	lst->type = type;
	return(0);
}

t_char_type get_char_type(char c)
{
	if (c == '\0')
		return (CHAR_NULL);
	else if (c == ' ')
		return (CHAR_SPACE);
	else if (c == '|' || c == '&' || c == '<' || c == '>' || c == '(' || c == ')' || c == '$')
		return (CHAR_OPERATOR);
	else if (c == '\'')
		return (CHAR_QUOTE_SINGLE);
	else if (c == '"')
		return (CHAR_QUOTE_DOUBLE);
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
		return (TOKEN_SPARAM);
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
	t_tok_type tok_type;

	while (data->tok->pos < data->tok->length)
	{
		data->tok->curr_char_type = get_char_type(data->tok->input[data->tok->pos]);
		// Handle the quote system.
		// if (data->tok->curr_char_type == CHAR_QUOTE_DOUBLE)
		// else if (data->tok->curr_char_type == CHAR_QUOTE_SINGLE)
		// Normal behavior, also be careful of the $
		if (data->tok->quote_state == QUOTE_NONE)
		{
			if (data->tok->curr_char_type != data->tok->prev_char_type)
			{
				if (data->tok->prev_char_type != CHAR_SPACE)
				{
					// Create with previous_char_type
					tok_type = get_tok_type(data->tok->input[data->tok->pos], check_next_char(data->tok->input, data->tok->pos));
					if (tok_type == TOKEN_HER)
					create_token(data, data->tok->prev_pos, data->tok->pos, tok_type);
					data->tok->prev_pos = data->tok->pos;
				}
			}
		}
		data->tok->pos++;
	}
	return(0);
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
