/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_random.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 19:11:39 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/23 16:01:26 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Read from random, else generate an int from a memory address.
int	gen_random(int size)
{
	int		fd;
	int		seed;
	char	*addr;

	if (size == 0)
		size = MATRIX;
	addr = NULL;
	seed = 0;
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		fd = open("/dev/random", O_RDONLY);
	if (fd == -1)
	{
		addr = malloc(1);
		seed = (int)(uintptr_t)addr;
		free(addr);
	}
	else
	{
		read(fd, &seed, sizeof(seed));
		close(fd);
	}
	return ((seed % size + 1) - size);
}
