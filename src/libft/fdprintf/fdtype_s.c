/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdtype_s.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:06:54 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/30 21:54:12 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdprintf.h"

int	fdtype_s(char *str, int fd)
{
	size_t	i;

	if (str == NULL)
	{
		fdtype_s("(null)", fd);
		return (6);
	}
	i = 0;
	while (str[i] != '\0')
	{
		write(fd, &str[i], 1);
		i++;
	}
	return (i);
}
