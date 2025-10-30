/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdprintf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 15:40:30 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/30 18:31:59 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdprintf.h"

int	fdrouter(char c, va_list list, int fd)
{
	int		i;
	size_t	y;

	i = 0;
	y = 0;
	if (c == 'c')
		i = fdtype_c((char)va_arg(list, int), fd);
	else if (c == 's')
		i = fdtype_s(va_arg(list, char *), fd);
	else if (c == 'i' || c == 'd')
		i = fdtype_i(va_arg(list, int), fd);
	else if (c == 'p')
		i = fdtype_p(va_arg(list, void *), fd);
	else if (c == 'x' || c == 'X')
		i = fdtype_xx(va_arg(list, unsigned int), c, fd);
	else if (c == 'u')
		i = fdtype_u(va_arg(list, unsigned int), fd);
	return (i + y);
}

int	fdloop(char *str, va_list list, int fd)
{
	size_t	i;
	size_t	y;
	size_t	nb;

	i = 0;
	y = 0;
	nb = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '%')
		{
			if (str[i + 1] == '%')
				y += write(fd, "%", 1);
			else
				y += fdrouter(str[i + 1], list, fd);
			nb += 2;
			i++;
		}
		else
			write(fd, &str[i], 1);
		i++;
	}
	i += y - nb;
	return (i);
}

int	fdprintf(int fd, const char *str, ...)
{
	int		count;
	va_list	list;

	if (fd < 0)
		return (-1);
	va_start(list, str);
	count = fdloop((char *)str, list, fd);
	va_end(list);
	return (count);
}
