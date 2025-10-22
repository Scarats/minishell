/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 18:41:50 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/22 18:44:05 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *cmd)
{
	int			i;
	int			size;
	const char	*builtins[] = {"echo", "cd", "pwd", "export",
		"unset", "env", "exit", "matrix", NULL};

	i = 0;
	size = ft_strlen(cmd);
	if (size < 1)
		return (0);
	while (builtins[i])
	{
		if (size == ft_strlen(builtins[i]) && ft_strncmp(builtins[i], cmd,
				(size_t)size) == 0)
			return (1);
		i++;
	}
	return (0);
}

// Check the conditions of what comes before and after '('
// returns 1 for error and 0 for no error.
// Prev token can be an operator or (.
int	check_left_par(t_token *tok_array, int index, int size)
{
	t_token_type	prev;

	if (index > 0)
	{
		prev = tok_array[index - 1].type;
		if (!is_op_or_redir(prev) && prev != TOKEN_LPAREN)
			return (1);
	}
	if (index + 1 >= size)
		return (1);
	else if (!is_word_token(tok_array[index + 1].type) && tok_array
		[index + 1].type != TOKEN_LPAREN)
		return (1);
	return (0);
}

// Check the conditions of what comes before and after ')'
// returns 1 for error and 0 for no error.
// Prev token can be a word or ).
int	check_right_par(t_token *tok_array, int index, int size)
{
	t_token_type	prev;

	if (index == 0)
		return (1);
	else if (index > 0)
	{
		prev = tok_array[index - 1].type;
		if (!is_word_token(prev) && prev != TOKEN_RPAREN)
			return (1);
	}
	if (index + 1 >= size)
		return (0);
	if (!is_op_or_redir(tok_array[index + 1].type) && tok_array
		[index + 1].type != TOKEN_RPAREN)
		return (1);
	return (0);
}

// Check for (a)(b), (((a) && b errors.
int	check_paren_error(t_token *tok_array, int size)
{
	int	i;
	int	depth;

	i = -1;
	depth = 0;
	while (++i < size)
	{
		if (depth < 0)
			return (1);
		else if (tok_array[i].type == TOKEN_LPAREN)
		{
			if (check_left_par(tok_array, i, size))
				return (1);
			depth++;
		}
		else if (tok_array[i].type == TOKEN_RPAREN)
		{
			if (check_right_par(tok_array, i, size))
				return (1);
			depth--;
		}
	}
	if (depth != 0)
		return (1);
	return (0);
}
