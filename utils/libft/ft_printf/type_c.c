/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_c.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:20:04 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/14 14:30:52 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	type_c(char c)
{
    write(1, &c, 1);
    return (1);
}

int	buffer_type_c(char c, t_buffer *s_buffer)
{
    write_char_to_buffer(c, s_buffer);
    return (1);
}
