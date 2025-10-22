/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_and.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:06:16 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/22 14:06:41 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	and_and(t_node *node, t_main_data *data)
{
	int	error;

	error = traverse_tree(node->left, data);
	if (error == 0)
		return (traverse_tree(node->right, data));
	return (error);
}
