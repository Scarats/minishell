/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 18:29:33 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/14 19:31:52 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	check_next_char(char *str, int pos)
{
	if (!str[pos] || !str[pos + 1])
		return ('\0');
	else
		return (str[pos + 1]);
}

// Create token, add to the list, malloc.
// if first node, set prev to NULL
t_token	*add_to_list(t_main_data *data, t_token *prev)
{
	t_token	*new;
	t_root	*root;

	root = data->root;
	new = my_malloc(&root->list_of_list, &data->malloc_tok, sizeof(t_token));
	if (!new)
		return (NULL);
	if (prev == NULL)
	{
		new->prev_token = NULL;
		data->tok->token_list = new;
	}
	else
	{
		prev->next_token = new;
		new->prev_token = prev;
	}
	data->tok->last_token = new;
	new->next_token = NULL;
	data->tok->token_list_size++;
	return (new);
}

// Set quote flags.
int	handle_quotes(t_tokenizer *tok, t_main_data *data, char c)
{
	if (c == '\'' && !tok->double_quote)
	{
		if (tok->single_quote && tok->prev_char_type == CHAR_SPACE)
		{
			create_token(data, tok->pos, tok->pos, TOKEN_TEXT);
			tok->prev_pos = tok->pos + 1;
		}
		tok->single_quote = !tok->single_quote;
		tok->curr_char_type = CHAR_SPACE;
	}
	else if (c == '"' && !tok->single_quote)
	{
		if (tok->double_quote && tok->prev_char_type == CHAR_SPACE)
		{
			create_token(data, tok->pos, tok->pos, TOKEN_TEXT);
			tok->prev_pos = tok->pos + 1;
		}
		tok->double_quote = !tok->double_quote;
		tok->curr_char_type = CHAR_SPACE;
	}
	else if ((tok->double_quote && c != '$') || tok->single_quote)
		tok->curr_char_type = CHAR_TEXT;
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
