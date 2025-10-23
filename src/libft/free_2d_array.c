/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2d_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:58:27 by tcardair          #+#    #+#             */
/*   Updated: 2025/09/24 16:29:29 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Free any type of 2d array.
void	free_2d_array(void **arr)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
