/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdtype_p.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:08:05 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/30 18:38:51 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdprintf.h"

int	fdtype_p(void *ptr, int fd)
{
	unsigned long long	i;
	unsigned long long	addr;

	if (ptr != NULL)
		fdtype_s("0x", fd);
	else
	{
		fdtype_s("(nil)", fd);
		return (5);
	}
	addr = (unsigned long long)ptr;
	i = fdto_hexa(addr, fd);
	return (i + 2);
}
/*
int	main(void)
{
	int a = 12;
	int *test = &a;
	type_p(test);
	return (0);
}*/