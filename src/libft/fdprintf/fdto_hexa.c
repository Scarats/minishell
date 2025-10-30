/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdto_hexa.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:30:26 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/30 18:38:41 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdprintf.h"

unsigned long long	fdto_hexa(unsigned long long addr, int fd)
{
	unsigned long long	count;

	count = 0;
	if (addr >= 16)
	{
		count += fdto_hexa(addr / 16, fd);
	}
	fdtype_c(HEXA[addr % 16], fd);
	count += 1;
	return (count);
}
