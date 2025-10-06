/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_xx.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:56:29 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/06 14:30:57 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	swap(char *ptr, int i)
{
	int		y;
	char	temp;

	y = 0;
	while (y < i / 2)
	{
		temp = ptr[y];
		ptr[y] = ptr[i - y - 1];
		ptr[i - y - 1] = temp;
		y++;
	}
}

int	type_xx(unsigned int nb, char c)
{
	int		i;
	int		y;
	char	*ptr;

	ptr = malloc(9 * (sizeof(char)));
	if (!ptr)
		return (-1);
	i = to_hexa_ptr(nb, ptr);
	if (c == 'x')
	{
		y = 0;
		while (ptr[y] != '\0')
		{
			if (ptr[y] <= 90 && ptr[y] >= 65)
				ptr[y] += 32;
			y++;
		}
	}
	type_s(ptr);
	free(ptr);
	return (i);
}

static char	get_hex_digit(unsigned int digit, char c)
{
	if (digit < 10)
		return (digit + '0');
	if (c == 'x')
		return (digit - 10 + 'a');
	return (digit - 10 + 'A');
}

static int	buffer_write_hex(unsigned int nb, char c, t_buffer *s_buffer)
{
	int	count;

	count = 0;
	if (nb >= 16)
	{
		count = buffer_write_hex(nb / 16, c, s_buffer);
		if (*(s_buffer->pos) < s_buffer->max_size - 1)
			s_buffer->buffer[(*(s_buffer->pos))++] = get_hex_digit(nb % 16, c);
		else
			(*(s_buffer->pos))++;
		return (count + 1);
	}
	if (*(s_buffer->pos) < s_buffer->max_size - 1)
		s_buffer->buffer[(*(s_buffer->pos))++] = get_hex_digit(nb, c);
	else
		(*(s_buffer->pos))++;
	return (1);
}

int	buffer_type_xx(unsigned int nb, char c, t_buffer *s_buffer)
{
	return (buffer_write_hex(nb, c, s_buffer));
}
