/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdtype_xx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:56:29 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/30 18:38:55 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdprintf.h"

void	fdswap(char *ptr, int i)
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

unsigned int	fdto_hexa_ptr(unsigned int nb, char *ptr)
{
	int		i;

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
	fdswap(ptr, i);
	return (i);
}

int	fdtype_xx(unsigned int nb, char c, int fd)
{
	int		i;
	int		y;
	char	*ptr;

	ptr = malloc(9 * (sizeof(char)));
	if (!ptr)
		return (-1);
	i = fdto_hexa_ptr(nb, ptr);
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
	fdtype_s(ptr, fd);
	free(ptr);
	return (i);
}
/*
#include <stdio.h>

int	main(void)
{
	unsigned int i = 3000000000;
	char c = 'x';
	int y = type_xX(i, c);
	printf("\n%d, %X", y, i);
	return (0);
}*/