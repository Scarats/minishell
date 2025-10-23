/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_hexa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:30:26 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/06 14:30:15 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

unsigned long long	to_hexa(unsigned long long addr)
{
	unsigned long long	count;

	count = 0;
	if (addr >= 16)
	{
		count += to_hexa(addr / 16);
	}
	type_c(HEXA[addr % 16]);
	count += 1;
	return (count);
}

unsigned int	to_hexa_ptr(unsigned int nb, char *ptr)
{
	int	i;

	i = 0;
	if (nb == 0)
		ptr[i++] = '0';
	else
	{
		while (nb > 0)
		{
			ptr[i++] = HEXA_MAJ[nb % 16];
			nb /= 16;
		}
	}
	ptr[i] = '\0';
	swap(ptr, i);
	return (i);
}
