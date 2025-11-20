/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:09:05 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/20 15:22:39 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int handle_operator(t_main_data *data, t_token_type *tok_type)
{
	int len;

	len = 0;
	if (!data || !tok_type)
		return (1);
	if (*tok_type == TOKEN_AND_AND || *tok_type == TOKEN_OR || *tok_type == TOKEN_HEREDOC || *tok_type == TOKEN_APPEND)
		len = 2;
	else
		len = 1;
	create_token(data, data->tok->prev_pos, data->tok->prev_pos + len,
				 *tok_type);
	data->tok->prev_pos += len;
	return (0);
}

int handle_parenthesis(t_main_data *data, t_token_type *tok_type)
{
	if (!data || !tok_type)
		return (1);
	create_token(data, data->tok->prev_pos, data->tok->pos, *tok_type);
	data->tok->prev_pos = data->tok->pos;
	return (0);
}

int quote_after_dollar(t_main_data *data)
{
	char next;

	if (data->tok->prev_pos + 1 < data->tok->length)
	{
		next = data->tok->input[data->tok->prev_pos + 1];
		if (next == '"' || next == '\'')
		{
			data->tok->prev_pos += 1;
			return (1);
		}
	}
	return (0);
}
