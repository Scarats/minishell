/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 18:41:18 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/05 18:46:16 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Check all the tokens are in parenthesis (tokens).
int	wrapped_in_paren(t_token *tok_array, int size)
{
	int	i;
	int	depth;

	i = -1;
	depth = 0;
	if (size < 2 || tok_array[0].type != TOKEN_LPAREN || tok_array
		[size - 1].type != TOKEN_RPAREN)
		return (0);
	if (size == 2)
		return (0);
	while (++i < size - 1)
	{
		if (tok_array[i].type == TOKEN_LPAREN)
			depth++;
		else if (tok_array[i].type == TOKEN_RPAREN)
			depth--;
		if (depth == 0 && i != size - 2)
			return (0);
	}
	return (depth == 1);
}

// Return the number of cmd and args in tok_array
int	get_cmd_argc(t_token *tok_array, int size)
{
	int	i;
	int	cmd_argc;

	i = 0;
	cmd_argc = 0;
	while (i < size)
	{
		if (tok_array[i].type == TOKEN_CMD
			|| tok_array[i].type == TOKEN_ARGUMENT
			|| tok_array[i].type == TOKEN_TEXT
			|| tok_array[i].type == TOKEN_ENV_VAR)
			cmd_argc++;
		i++;
	}
	return (cmd_argc);
}

// take token type and return node type.
t_node_type	map_token_to_node(t_token_type t)
{
	if (t == TOKEN_PIPE)
		return (NODE_PIPE);
	if (t == TOKEN_AND_AND)
		return (NODE_AND);
	if (t == TOKEN_OR)
		return (NODE_OR);
	return (NODE_COMMAND);
}

// Create new node to the list, malloc and memset it, add its type.
// If first, initialize the list.
t_redir	*add_redirection(t_main_data *data, t_node *node, t_token_type type)
{
	t_redir	*redirection;
	t_redir	*last;
	t_root	*root;

	root = data->root;
	redirection = my_malloc(&root->list_of_list, &data->malloc_tree,
			sizeof(t_redir));
	redirection->type = type;
	redirection->heredoc_fd = -1;
	if (!node->redirection)
		node->redirection = redirection;
	else
	{
		last = node->redirection;
		while (last->next)
			last = last->next;
		last->next = redirection;
	}
	return (redirection);
}
