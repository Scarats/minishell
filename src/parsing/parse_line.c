#include "../../minishell.h"

// Determine if the word is a command, argument, filename etc...
// It will be done according to the previous tokens created.
t_char_type get_word_type()
{

}

int	create_token(t_main_data *data, int start, int end, t_token_type type)
{
}

t_char_type	get_char_type(char c)
{
	if (c == '\0')
		return (CHAR_NULL);
	else if (c == ' ')
		return (CHAR_SPACE);
	else if (c == '|' || c == '&' || c == '<' || c == '>' || c == '(' || c == ')')
		return (CHAR_OPERATOR);
	else if (c == '\'')
		return (CHAR_QUOTE_SINGLE);
	else if (c == '"')
		return (CHAR_QUOTE_DOUBLE);
	return (CHAR_TEXT);
}

t_token_type	get_tok_type(char c, char next)
{
	if (c == '&' && next == '&')
		return (TOKEN_AND_AND);
	if (c == '|' && next == '|')
		return (TOKEN_OR);
	if (c == '<' && next == '<')
		return (TOKEN_HEREDOC);
	if (c == '>' && next == '>')
		return (TOKEN_APPEND);
	if (c == '&')
		return (TOKEN_AND);
	if (c == '|')
		return (TOKEN_PIPE);
	if (c == '<')
		return (TOKEN_REDIRECT_IN);
	if (c == '>')
		return (TOKEN_REDIRECT_OUT);
	if (c == '(')
		return (TOKEN_LPAREN);
	if (c == ')')
		return (TOKEN_RPAREN);
	if (c == ' ')
		return (TOKEN_SPACE);
	if (c == '\0')
		return (TOKEN_NULL);
	return (TOKEN_TEXT);
}

int	tokenizer(t_main_data *data)
{
	while (data->tok->pos < data->tok->length)
	{
		data->tok->curr_char_type = get_char_type(data->tok->input[data->tok->pos]);
		// Handle the quote system.
		if (data->tok->curr_char_type == CHAR_QUOTE_DOUBLE)
		else if (data->tok->curr_char_type == CHAR_QUOTE_SINGLE)

		// Normal behavior, also be careful of the $
		else if (data->tok->quote_state == QUOTE_NONE)
		{
			if (data->tok->curr_char_type != data->tok->prev_char_type)
			{
				// Create with previous_char_type
				create_token();
				data->tok->prev_pos = data->tok->pos;
			}

		}

		data->tok->pos++;
	}
}

// Parse the input.
// Tokenize it, then create a binary tree.
int	parser(t_main_data *data)
{
	if (tokenizer(data))
		return (1);
	if (build_tree(data))
		return (1);
	return (0);
}
