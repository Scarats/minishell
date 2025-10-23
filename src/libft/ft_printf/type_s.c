/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_s.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:06:54 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/14 14:51:48 by aadeikal         ###   ########.fr       */
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

int	buffer_type_s(char *str, t_buffer *s_buffer)
{
	int	count;

	if (!str)
		str = "(null)";
	count = 0;
	while (*str)
	{
		write_char_to_buffer(*str, s_buffer);
		str++;
		count++;
	}
	return (count);
}
