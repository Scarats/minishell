#include "ft_printf.h"

int buffer_type_s(char c, char *buffer, size_t pos, size_t max_size)
{
    size_t = i;

    i = 0;
    if (str == NULL)
        return buffer_type_s("(null)", buffer, pos, max_size);
    while(str[i])
    {
        if (*pos < max_size - 1)
            buffer[(*pos)++] = str[i];
        else
            (*pos)++;
        i++;
    }
    return (i);
}
