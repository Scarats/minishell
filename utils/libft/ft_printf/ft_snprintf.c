/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:49:27 by aadeikal          #+#    #+#             */
/*   Updated: 2025/10/06 15:01:05 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	write_char_to_buffer(char c, t_buffer *s_buffer)
{
	if (s_buffer->buffer != NULL && *(s_buffer->pos) < s_buffer->max_size - 1)
		s_buffer->buffer[(*(s_buffer->pos))] = c;
	(*(s_buffer->pos))++;
}

int	handle_percent(char *str, size_t *i, va_list list, t_buffer *s_buffer)
{
	int	chars_added;

	chars_added = 0;
	if (!str || !s_buffer)
		return (-1);
	if (str[*i + 1] == '\0')
		return (-1);
	if (str[*i + 1] == '%')
	{
		write_char_to_buffer('%', s_buffer);
		chars_added = 1;
	}
	else
	{
		chars_added = buffer_router(str[*i + 1], list, s_buffer);
		if (chars_added < 0)
			return (-1);
	}
	(*i) += 2;
	return (chars_added);
}

void	null_terminate_buffer(t_buffer *s_buffer)
{
	if (!s_buffer)
		return ;
	if (s_buffer->buffer == NULL || s_buffer->max_size == 0)
		return ;
	if (*(s_buffer->pos) < s_buffer->max_size - 1)
		s_buffer->buffer[*(s_buffer->pos)] = '\0';
	else
		s_buffer->buffer[s_buffer->max_size - 1] = '\0';
}

int	ft_snprintf(char *buffer, size_t max_size, const char *format, ...)
{
	int		result;
	va_list	list;

	if (!format)
		return (-1);
	va_start(list, format);
	result = buffer_loop((char *)format, list, buffer, max_size);
	va_end(list);
	if (result < 0)
		return (-1);
	return (result);
}
