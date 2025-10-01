/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_s.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:06:54 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/01 15:34:34 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	type_s(char *str)
{
	size_t	i;

	if (str == NULL)
	{
		type_s("(null)");
		return (6);
	}
	i = 0;
	while (str[i] != '\0')
	{
		write(1, &str[i], 1);
		i++;
	}
	return (i);
}

int	buffer_type_s(char c, char *buffer, size_t pos, size_t max_size)
{
	size_t	i;

	i = 0;
	if (str == NULL)
		return (buffer_type_s("(null)", buffer, pos, max_size));
	while (str[i])
	{
		if (*pos < max_size - 1)
			buffer[(*pos)++] = str[i];
		else
			(*pos)++;
		i++;
	}
	return (i);
}
