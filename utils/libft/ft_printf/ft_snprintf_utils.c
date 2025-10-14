/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:51:17 by aadeikal          #+#    #+#             */
/*   Updated: 2025/10/14 14:28:13 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	init_buffer_context(t_buffer *s_buffer, char *buffer, size_t *pos,
        size_t max_size)
{
    *pos = 0;
    s_buffer->buffer = buffer;
    s_buffer->pos = pos;
    s_buffer->max_size = max_size;
}

static int	process_format_specifier(char *str, size_t *i, va_list list,
        t_buffer *s_buffer)
{
    int	result;

    result = handle_percent(str, i, list, s_buffer);
    if (result < 0)
        return (-1);
    return (result);
}

static void	process_regular_char(char c, t_buffer *s_buffer, int *total_chars,
        size_t *i)
{
    write_char_to_buffer(c, s_buffer);
    (*total_chars)++;
    (*i)++;
}

int	buffer_loop(char *str, va_list list, char *buffer, size_t max_size)
{
    size_t		i;
    size_t		pos;
    int			total_chars;
    t_buffer	s_buffer;
    int			result;

    i = 0;
    total_chars = 0;
    init_buffer_context(&s_buffer, buffer, &pos, max_size);
    if (!str)
        return (-1);
    while (str[i] != '\0')
    {
        if (str[i] == '%')
        {
            result = process_format_specifier(str, &i, list, &s_buffer);
            if (result < 0)
                return (-1);
            total_chars += result;
        }
        else
            process_regular_char(str[i], &s_buffer, &total_chars, &i);
    }
    null_terminate_buffer(&s_buffer);
    return (total_chars);
}

int	buffer_router(char c, va_list list, t_buffer *s_buffer)
{
    int	chars_would_write;

    chars_would_write = 0;
    if (c == 'c')
        chars_would_write = buffer_type_c((char)va_arg(list, int), s_buffer);
    else if (c == 's')
        chars_would_write = buffer_type_s(va_arg(list, char *), s_buffer);
    else if (c == 'i' || c == 'd')
        chars_would_write = buffer_type_i(va_arg(list, int), s_buffer);
    else if (c == 'p')
        chars_would_write = buffer_type_p(va_arg(list, void *), s_buffer);
    else if (c == 'x' || c == 'X')
        chars_would_write = buffer_type_xx(va_arg(list, unsigned int), c,
                s_buffer);
    else if (c == 'u')
        chars_would_write = buffer_type_u(va_arg(list, unsigned int), s_buffer);
    else
        return (-1);
    return (chars_would_write);
}
