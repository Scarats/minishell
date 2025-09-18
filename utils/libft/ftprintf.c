/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftprintf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 15:40:30 by tcardair          #+#    #+#             */
/*   Updated: 2025/09/18 15:40:31 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf/ft_printf.h"

int	fdprintf(int fd, const char *str, ...)
{
	int		count;
	va_list	list;
	int		saved_stdout;

	if (fd < 0)
		return (-1);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
		return (-1);
	if (fd != STDOUT_FILENO)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (close(saved_stdout), -1);
	}
	va_start(list, str);
	count = loop((char *)str, list);
	va_end(list);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		return (close(saved_stdout), -1);
	return (close(saved_stdout), count);
}
