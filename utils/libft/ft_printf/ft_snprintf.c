#include "ft_printf.h"

static void write_char_to_buffer(char c, char *buffer, size_t *pos, size_t max_size)
{
    if (*pos < max_size - 1)
        buffer[(*pos)++] = c;
    else
        (*pos)++;
}

static int handle_percent(char *str, size_t *i, va_list list, 
                         char *buffer, size_t *pos, size_t max_size)
{
    int chars_added = 0;
    
    if (str[*i + 1] == '%')
    {
        write_char_to_buffer('%', buffer, pos, max_size);
        chars_added = 1;
    }
    else
        chars_added = buffer_router(str[*i + 1], list, buffer, pos, max_size);
    
    (*i) += 2;
    return chars_added;
}

static void null_terminate_buffer(char *buffer, size_t pos, size_t max_size)
{
    if (max_size > 0)
    {
        if (pos < max_size - 1)
            buffer[pos] = '\0';
        else
            buffer[max_size - 1] = '\0';
    }
}

int buffer_router(char c, va_list list, char *buffer, size_t *pos, size_t max_size)
{
    int chars_would_write = 0;
    
    if (c == 'c')
        chars_would_write = buffer_type_c((char)va_arg(list, int), buffer, pos, max_size);
    else if (c == 's')
        chars_would_write = buffer_type_s(va_arg(list, char *), buffer, pos, max_size);
    else if (c == 'i' || c == 'd')
        chars_would_write = buffer_type_i(va_arg(list, int), buffer, pos, max_size);
    else if (c == 'p')
		chars_would_write = buffer_type_p(va_arg(list, int), buffer, pos, max_size);
	else if (c == 'x' || c == 'X')
		chars_would_write = buffer_type_xx(va_arg(list, int), buffer, pos, max_size);
	else if (c == 'u')
		chars_would_write = buffer_type_u(va_arg(list, int), buffer, pos, max_size);    
    return (chars_would_write);
}

int buffer_loop(char *str, va_list list, char *buffer, size_t max_size)
{
    size_t i = 0;
    size_t pos = 0;
    int total_chars = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '%')
            total_chars += handle_percent(str, &i, list, buffer, &pos, max_size);
        else
        {
            write_char_to_buffer(str[i], buffer, &pos, max_size);
            total_chars++;
            i++;
        }
    }
    
    null_terminate_buffer(buffer, pos, max_size);
    return (total_chars);
}

int ft_snprintf(char *buffer, size_t max_size, const char *format, ...)
{
    int result;
    va_list list;
    
    va_start(list, format);
    result = buffer_loop((char *)format, list, buffer, max_size);
    va_end(list);
    
    return result;
}
