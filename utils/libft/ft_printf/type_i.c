/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_i.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:36:10 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/01 15:36:22 by aadeikal         ###   ########.fr       */
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

int	buffer_type_i(int nb, char *buffer, size_t *pos, size_t max_size)
{
	int	count;

	count = 0;
	if (nb == -2147483648)
		return (buffer_type_s("-2147483648", buffer, pos, max_size));
	if (nb < 0)
	{
		buffer_type_c('-', buffer, pos, max_size);
		count = buffer_type_i(-nb, buffer, pos, max_size) + 1;
	}
	else if (nb >= 10)
	{
		count = buffer_type_i(nb / 10, buffer, pos, max_size) + 1;
		buffer_type_c(nb % 10 + '0', buffer, pos, max_size);
	}
	else
	{
		buffer_type_c(nb + '0', buffer, pos, max_size);
		count = 1;
	}
	return (count);
}
