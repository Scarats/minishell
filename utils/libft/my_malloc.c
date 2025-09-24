/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:08:18 by tcardair          #+#    #+#             */
/*   Updated: 2025/09/24 16:23:44 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Add ptr to the list or initialiaze the new list.
void add_or_create_list(t_list **list, void *ptr)
{
	if (!*list)
		*list = ft_lstnew(ptr);
	else
		ft_lstadd_back(list, ft_lstnew(ptr));
}

// Same usage as malloc, but keep track of allocated memory in list.
// Memset memory.
// list_of_list keep track of all the malloc_list of the program.
// In the case of a malloc error, it will free all the allocated memory of the program and exit() cleanly.
void *my_malloc(t_list **list_of_list, t_list **malloc_list, size_t size)
{
	void *ptr;

	if (size < 1)
		return (NULL);
	if (list_of_list && malloc_list && !check_list(list_of_list, malloc_list))
		add_or_create_list(list_of_list, malloc_list);
	ptr = ft_calloc(1, size);
	if (!ptr)
	{
		if (list_of_list)
			my_multi_free(list_of_list);
		else
			my_free(malloc_list);
		exit(1);
	}
	if (malloc_list)
		add_or_create_list(malloc_list, ptr);
	return (ptr);
}

// Free a list of malloc_list.
void my_multi_free(t_list **list_of_list)
{
    t_list *node;

    if (!list_of_list || !*list_of_list)
        return;

    node = *list_of_list;
    while (node)
    {
        if (node->content)
            my_free((t_list **)node->content);
        node = node->next;
    }
    ft_lstclear(list_of_list, NULL);
}

// Free the allocated memory of the list.
void my_free(t_list **list)
{
	if (list && *list)
	{
		ft_lstclear(list, free);
		*list = NULL;
	}
}

// Check if it has already been allocated
// and added to the list (avoid double free).
int check_list(t_list **list, void *data)
{
	t_list *temp;

	if (!list || !*list || !data)
		return (0);
	temp = *list;
	while (temp)
	{
		if (temp->content == data)
			return (1);
		temp = temp->next;
	}
	return (0);
}

// Add already allocated memory to the list.
void my_addtolist(t_list **list, void *data)
{
	t_list *temp;

	if (!(list && data))
		return;
	if (check_list(list, data) == 1)
		return;
	temp = ft_lstnew(data);
	ft_lstadd_back(list, temp);
}

// Add the ptr of the array and each element of the array to the list.
void my_array_addtolist(t_list **list, void **data)
{
	unsigned long index;

	if (!(list && data))
		return;
	index = 0;
	my_addtolist(list, data);
	while (data[index])
	{
		my_addtolist(list, data[index]);
		index++;
	}
}
