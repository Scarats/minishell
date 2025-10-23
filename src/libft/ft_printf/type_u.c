/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_u.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 22:53:27 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/06 14:24:45 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	type_u(unsigned int nb)
{
	unsigned long	count;

	count = 0;
	if (nb < 0)
	{
		type_c('-');
		count = type_i(-nb) + 1;
	}
	else if (nb >= 10)
	{
		count = type_i(nb / 10) + 1;
		type_c(nb % 10 + '0');
	}
	else
	{
		type_c(nb + '0');
		count += 1;
	}
	return (count);
}

int	buffer_type_u(unsigned int nb, t_buffer *s_buffer)
{
	int	count;

	if (nb >= 10)
	{
		count = buffer_type_u(nb / 10, s_buffer);
		if (*(s_buffer->pos) < s_buffer->max_size - 1)
			s_buffer->buffer[(*(s_buffer->pos))++] = (nb % 10) + '0';
		else
			(*(s_buffer->pos))++;
		return (count + 1);
	}
	if (*(s_buffer->pos) < s_buffer->max_size - 1)
		s_buffer->buffer[(*(s_buffer->pos))++] = nb + '0';
	else
		(*(s_buffer->pos))++;
	return (1);
}
