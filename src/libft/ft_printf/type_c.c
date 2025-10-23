/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_c.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:20:04 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/23 15:59:16 by tcardair         ###   ########.fr       */
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
