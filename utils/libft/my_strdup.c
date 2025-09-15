/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:12:02 by tcardair          #+#    #+#             */
/*   Updated: 2025/09/15 10:32:00 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// strdup and add the allocated memory to the list.
char	*my_strdup(t_list **malloc_list, char *src)
{
	char	*dup;

	dup = ft_strdup(src);
	if (!dup)
		return (NULL);
	my_addtolist(malloc_list, dup);
	return (dup);
}
