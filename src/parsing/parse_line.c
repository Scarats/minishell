#include "../../minishell.h"

// Determine if the word is a command, argument, filename etc...
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
	if (c == ' ')
		return (CHAR_SPACE);
	if (c == '|' || c == '&' || c == '<' || c == '>' || c == '(' || c == ')')
		return (CHAR_OPERATOR);
	return (CHAR_WORD);
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
