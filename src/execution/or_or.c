/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   or_or.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:52:30 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/22 14:52:40 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	or_or(t_node *node, t_main_data *data)
{
	int	error;

	error = traverse_tree(node->left, data);
	if (error)
		return (traverse_tree(node->right, data));
	return (error);
}
