/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:02:26 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/22 18:40:21 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	handle_normal_token(t_main_data *data, t_token_type *tok_type)
{
	t_token_type	effective;

	if (!data || !tok_type)
		return (1);
	if (data->tok->prev_char_type == CHAR_OPERATOR
		&& data->tok->prev_pos < data->tok->pos
		&& data->tok->input[data->tok->prev_pos] == '$')
	{
		create_token(data, data->tok->prev_pos, data->tok->prev_pos + 1,
			TOKEN_DOLLAR);
		data->tok->prev_pos = data->tok->prev_pos + 1;
		return (0);
	}
	if (data->tok->prev_char_type != CHAR_SPACE
		&& data->tok->prev_pos < data->tok->pos)
	{
		effective = *tok_type;
		if (data->tok->prev_char_type == CHAR_TEXT)
			effective = TOKEN_TEXT;
		create_token(data, data->tok->prev_pos, data->tok->pos, effective);
	}
	data->tok->prev_pos = data->tok->pos;
	return (0);
}

// Create token, add them to the list and add type.
int	create_token(t_main_data *data, int start, int end, t_token_type type)
{
	t_token	*tok;
	t_token	*removed;
	int		removed_dollar;

	removed_dollar = 0;
	if (type == TOKEN_SPACE)
		type = TOKEN_TEXT;
	tok = add_to_list(data, data->tok->last_token);
	if (!tok)
		return (1);
	if (type == TOKEN_TEXT)
		type = get_word_type(tok);
	tok->type = type;
	removed = tok->prev_token;
	tok->slice = ft_substr(data->tok->input, start, end - start);
	if (tok->slice)
		my_addtolist(&data->malloc_tok, tok->slice);
	tok->word = tok->slice;
	if (tok->type == TOKEN_ENV_VAR)
		expand_var(data, tok, removed, &removed_dollar);
	merge_with_prev_if_adjacent(data, tok, start, removed_dollar);
	return (0);
}

void	tokenizer_logic(t_main_data *data, t_token_type *tok_type)
{
	if (data->tok->curr_char_type == CHAR_PARENTHESIS
		&& data->tok->prev_char_type == CHAR_PARENTHESIS
		&& data->tok->pos > data->tok->prev_pos)
		handle_parenthesis(data, tok_type);
	else if (data->tok->curr_char_type == CHAR_OPERATOR
		&& data->tok->prev_char_type == CHAR_OPERATOR
		&& data->tok->pos > data->tok->prev_pos)
		handle_operator(data, tok_type);
	else if (data->tok->curr_char_type != data->tok->prev_char_type)
		handle_normal_token(data, tok_type);
	data->tok->prev_char_type = data->tok->curr_char_type;
	data->tok->pos++;
}

int	tokenizer(t_main_data *data)
{
	t_token_type	tok_type;

	tok_type = TOKEN_NULL;
	while (data->tok->pos < data->tok->length)
	{
		data->tok->curr_char_type = get_char_type(data->tok->input
			[data->tok->pos]);
		handle_quotes(data->tok, data);
		tok_type = get_tok_type(data->tok->input[data->tok->prev_pos],
				check_next_char(data->tok->input, data->tok->prev_pos));
		tokenizer_logic(data, &tok_type);
	}
	if (data->tok->prev_char_type != CHAR_SPACE
		&& data->tok->prev_pos < data->tok->pos
		&& data->tok->prev_pos < data->tok->length && !data->tok->single_quote)
	{
		tok_type = get_tok_type(data->tok->input[data->tok->prev_pos],
				check_next_char(data->tok->input, data->tok->prev_pos));
		create_token(data, data->tok->prev_pos, data->tok->pos, tok_type);
	}
	if (data->tok->double_quote || data->tok->single_quote)
		return (fdprintf(2, "minishell: syntax error: unclosed quote\n"), 1);
	return (0);
}
