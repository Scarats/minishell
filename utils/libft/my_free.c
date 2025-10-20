/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:27:19 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/20 18:29:26 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Free only one node of the list.
void my_free_one(t_list **list, void *target)
{
    t_list *prev;
    t_list *next;
    t_list *curr;

    if (!list || !*list || !target)
        return ;
    curr = *list;
    if (curr->content == target)
    {
        next = curr->next;
        ft_lstdelone(curr, free);
        *list = next;
        return ;
    }
    prev = *list;
    while (prev->next && prev->next->content != target)
        prev = prev->next;
    if (!prev->next)
        return ;
    curr = prev->next;
    next = curr->next;
    ft_lstdelone(curr, free);
    prev->next = next;
}

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
