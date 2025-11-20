/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tok_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 18:38:51 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/20 14:12:07 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_operator(t_token_type t)
{
	return (t == TOKEN_PIPE || t == TOKEN_AND_AND || t == TOKEN_OR || t == TOKEN_AND);
}

int	is_op_or_redir(t_token_type t)
{
	return (t == TOKEN_PIPE || t == TOKEN_AND_AND || t == TOKEN_OR
		|| t == TOKEN_REDIRECT_IN || t == TOKEN_REDIRECT_OUT
		|| t == TOKEN_APPEND || t == TOKEN_HEREDOC);
}

int	is_redir(t_token_type t)
{
	return (t == TOKEN_REDIRECT_IN || t == TOKEN_REDIRECT_OUT
		|| t == TOKEN_APPEND || t == TOKEN_HEREDOC);
}

// Check if && or ||
int	is_and_or(t_token_type t)
{
	return (t == TOKEN_AND_AND || t == TOKEN_OR);
}

int	is_word_token(t_token_type t)
{
	return (t == TOKEN_CMD || t == TOKEN_ARGUMENT || t == TOKEN_FILE
		|| t == TOKEN_ENV_VAR || t == TOKEN_TEXT);
}
