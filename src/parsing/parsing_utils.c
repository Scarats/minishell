/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:05:42 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/22 15:06:56 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	else if (c == '\'')
		return (TOKEN_SINGLE_QUOTE);
	else if (c == '"')
		return (TOKEN_DOUBLE_QUOTE);
	return (TOKEN_TEXT);
}

// Turn the token linked list in an array, easier for AST.
int	list_to_array(t_main_data *data, t_token *token_list, int size)
{
	t_token	*curr_tok;
	int		i;
	int		real_size;

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
	if (size < 1)
		size = 1;
	data->tok->token_array = my_malloc(&data->root->list_of_list,
			&data->malloc_tok, sizeof(t_token) * size);
	if (!data->tok->token_array && size > 0)
		return (1);
	curr_tok = token_list;
	while (curr_tok && i < size)
	{
		data->tok->token_array[i++] = *curr_tok;
		curr_tok = curr_tok->next_token;
	}
	return (0);
}
