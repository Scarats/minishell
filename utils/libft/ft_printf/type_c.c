/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_c.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:20:04 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/01 15:35:02 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	type_c(char c)
{
	write(1, &c, 1);
	return (1);
}

int	buffer_type_c(char c, char *buffer, size_t pos, size_t max_size)
{
	if (*pos < max_size - 1)
		buffer[(*pos)++] = c;
	else
		(*pos)++;
	return (1);
}
