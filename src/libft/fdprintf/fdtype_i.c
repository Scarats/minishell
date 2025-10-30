/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdtype_i.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:36:10 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/30 21:50:06 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdprintf.h"

int	fdtype_i(int nb, int fd)
{
	int	count;

	count = 0;
	if (nb == -2147483648)
	{
		fdtype_s("-2147483648", fd);
		return (11);
	}
	else if (nb < 0)
	{
		fdtype_c('-', fd);
		count = fdtype_i(-nb, fd) + 1;
	}
	else if (nb >= 10)
	{
		count = fdtype_i(nb / 10, fd) + 1;
		fdtype_c(nb % 10 + '0', fd);
	}
	else
	{
		fdtype_c(nb + '0', fd);
		count += 1;
	}
	return (count);
}
