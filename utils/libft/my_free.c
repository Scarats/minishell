/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:27:19 by tcardair          #+#    #+#             */
/*   Updated: 2025/09/24 16:29:59 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Free the allocated memory of the list.
void	my_free(t_list **list)
{
	if (list && *list)
	{
		ft_lstclear(list, free);
		*list = NULL;
	}
}

// Free a list of malloc_list.
void	my_multi_free(t_list **list_of_list)
{
	t_list	*node;

	if (!list_of_list || !*list_of_list)
		return ;
	node = *list_of_list;
	while (node)
	{
		if (node->content)
			my_free((t_list **)node->content);
		node = node->next;
	}
	ft_lstclear(list_of_list, NULL);
}
