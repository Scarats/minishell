/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 18:49:36 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/22 18:55:11 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* scan for && / || (most significant), return index or -1 */
int	find_and_or(t_token *tok_array, int size)
{
	int	depth;
	int	pos;

	depth = 0;
	pos = size;
	while (--pos >= 0)
	{
		if (tok_array[pos].type == TOKEN_LPAREN)
			depth--;
		else if (tok_array[pos].type == TOKEN_RPAREN)
			depth++;
		else if (depth == 0 && is_and_or(tok_array[pos].type))
			return (pos);
	}
	return (-1);
}

/* scan for | (most significant), return index or -1 */
int	find_pipe(t_token *tok_array, int size)
{
	int	depth;
	int	pos;

	depth = 0;
	pos = size;
	while (--pos >= 0)
	{
		if (tok_array[pos].type == TOKEN_LPAREN)
			depth--;
		else if (tok_array[pos].type == TOKEN_RPAREN)
			depth++;
		else if (depth == 0 && tok_array[pos].type == TOKEN_PIPE)
			return (pos);
	}
	return (-1);
}

/* Return the index of the most significant operator.
   (&&, ||, |)
   If none found, return (-1); */
int	find_operator(t_token *tok_array, int size)
{
	int	pos;

	pos = find_and_or(tok_array, size);
	if (pos >= 0)
		return (pos);
	return (find_pipe(tok_array, size));
}

/* internal recursive builder (core logic) */
t_node	*build_tree_inner(t_main_data *data, t_token *tok_array, int size,
		int depth)
{
	int		i;
	t_node	*node;

	node = NULL;
	i = find_operator(tok_array, size);
	if (i < 0)
	{
		node = create_node(data, tok_array,
				map_token_to_node(tok_array[0].type), size);
		node->in_subshell = depth;
		return (node);
	}
	node = create_node(data, NULL, map_token_to_node(tok_array[i].type), 0);
	node->in_subshell = depth;
	node->left = build_tree(data, tok_array, i, depth);
	node->right = build_tree(data, tok_array + i + 1, size - (i + 1), depth);
	return (node);
}

/* make a recursive function that will find the most significant operator.
   create a node from it.
   Call itself from the first token of the left part,
   and call itself from the first token of the right part.
   Continue until no operator is found. */
t_node	*build_tree(t_main_data *data, t_token *tok_array, int size, int depth)
{
	t_node	*node;

	if (size <= 0 || !tok_array || check_paren_error(tok_array, size))
		return (NULL);
	if (wrapped_in_paren(tok_array, size))
	{
		node = build_tree_inner(data, tok_array + 1, size - 2, depth + 1);
		node->create_subshell = true;
		node->in_subshell = depth + 1;
		return (node);
	}
	return (build_tree_inner(data, tok_array, size, depth));
}
