/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_i.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:36:10 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/06 14:18:14 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	type_i(int nb)
{
	int	count;

	count = 0;
	if (nb == -2147483648)
	{
		type_s("-2147483648");
		return (11);
	}
	else if (nb < 0)
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

static int	buffer_type_i_recursive(int nb, t_buffer *s_buffer)
{
	int	count;

	count = 0;
	if (nb >= 10)
	{
		count = buffer_type_i_recursive(nb / 10, s_buffer);
		write_char_to_buffer((nb % 10) + '0', s_buffer);
		return (count + 1);
	}
	write_char_to_buffer(nb + '0', s_buffer);
	return (1);
}

int	buffer_type_i(int nb, t_buffer *s_buffer)
{
	int	count;

	count = 0;
	if (nb == -2147483648)
		return (buffer_type_s("-2147483648", s_buffer));
	if (nb < 0)
	{
		write_char_to_buffer('-', s_buffer);
		count = 1;
		nb = -nb;
	}
	return (count + buffer_type_i_recursive(nb, s_buffer));
}
