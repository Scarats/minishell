/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:27:19 by tcardair          #+#    #+#             */
/*   Updated: 2025/09/25 18:10:04 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void noop_del(void *p) { (void)p; }

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
    // Free the outer tracking nodes themselves (use noop_del so ft_lstclear removes nodes)
    ft_lstclear(list_of_list, noop_del);
}
