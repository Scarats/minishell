/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_p.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:08:05 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/06 15:10:33 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	type_p(void *ptr)
{
	unsigned long long	i;
	unsigned long long	addr;

	if (ptr != NULL)
		type_s("0x");
	else
	{
		type_s("(nil)");
		return (5);
	}
	addr = (unsigned long long)ptr;
	i = to_hexa(addr);
	return (i + 2);
}

static int	buffer_write_ptr_prefix(t_buffer *s_buffer)
{
	if (*(s_buffer->pos) < s_buffer->max_size - 1)
		s_buffer->buffer[(*(s_buffer->pos))++] = '0';
	else
		(*(s_buffer->pos))++;
	if (*(s_buffer->pos) < s_buffer->max_size - 1)
		s_buffer->buffer[(*(s_buffer->pos))++] = 'x';
	else
		(*(s_buffer->pos))++;
	return (2);
}

int	buffer_type_p(void *ptr, t_buffer *s_buffer)
{
	uintptr_t	addr;
	int			count;

	count = 0;
	if (ptr == NULL)
		return (buffer_type_s("(nil)", s_buffer));
	addr = (uintptr_t)ptr;
	count = buffer_write_ptr_prefix(s_buffer);
	count += buffer_type_xx(addr, 'x', s_buffer);
	return (count);
}
