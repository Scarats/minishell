/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:08:18 by tcardair          #+#    #+#             */
/*   Updated: 2025/09/02 17:27:01 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Same usage as malloc, but keep track of allocated memory in list.
// memset memory.
void	*my_malloc(t_list **list, size_t size)
{
	void	*ptr;

	ptr = ft_calloc(1, size);
	if (!ptr)
		return (NULL);
	if (list)
	{
		if (!*list)
			*list = ft_lstnew(ptr);
		else
			ft_lstadd_back(list, ft_lstnew(ptr));
	}
	return (ptr);
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

// Check if it has already been allocated and added to the list (avoid double free).
int	check_list(t_list **list, void *data)
{
	t_list	*temp;

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
void	my_addtolist(t_list **list, void *data)
{
	t_list	*temp;

	if (!(list && data))
		return ;
	if (check_list(list, data) == 1)
		return ;
	temp = ft_lstnew(data);
	ft_lstadd_back(list, temp);
}

// Add the ptr of the array and each element of the array to the list.
void	my_array_addtolist(t_list **list, void **data)
{
	unsigned long index;

	if (!(list && data))
		return ;
	index = 0;
	my_addtolist(list, data);
	while(data[index])
	{
		my_addtolist(list, data[index]);
		index++;
	}
}
