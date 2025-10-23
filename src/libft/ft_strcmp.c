/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 16:30:34 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/23 16:00:50 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Return 0 if s1 and s2 are identicals.
int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*ps1;
	unsigned char	*ps2;

	ps1 = (unsigned char *)s1;
	ps2 = (unsigned char *)s2;
	while (*ps1 && *ps1 == *ps2)
	{
		ps1++;
		ps2++;
	}
	return (*ps1 - *ps2);
}
