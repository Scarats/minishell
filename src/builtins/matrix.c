/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:02:19 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/23 13:07:48 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Apply random numbers within height and -height to an array of int.
void	gen_matrix(int *array, int width, int height)
{
	int	i;
	int	prev;
	int	range;

	i = 1;
	array[0] = gen_random(width);
	prev = array[0];
	while (i < width)
	{
		if ((height - prev) <= 0)
			range = prev - height;
		else
			range = height - prev;
		array[i] = gen_random(range);
		prev = array[i];
		i++;
	}
}

// Add random char if array[i] is positive, else add space.
void	fill_buff(int *array, char *buff, int width, int height)
{
	int			i;
	int			r;
	const int	printable_count = 126 - 33 + 1;

	i = -1;
	while (++i < width)
	{
		if (array[i] > 0)
		{
			r = gen_random(height);
			if (r < 0)
				r = -r;
			if (r == 0)
				r = 1;
			buff[i] = (char)(33 + (r % printable_count));
		}
		else
			buff[i] = ' ';
		if (array[i] >= height)
			array[i] = -height;
		else
			array[i] += 1;
	}
	buff[width] = '\0';
}

void	print_matrix(int *array, char *buff, int width, int height)
{
	int	i;

	i = 0;
	while (!g_stop_flag)
	{
		fill_buff(array, buff, width, height);
		printf(GREEN "%s\n" RESET, buff);
		if (i == 1000)
		{
			gen_matrix(array, width, height);
			i = 0;
		}
		else
			usleep(20000);
		i++;
	}
}

void	matrix_rest(int height, int width, int *array, char *buff)
{
	if (height == 0)
		height = MATRIX;
	printf(GREEN "\n\nFollow the white rabbit.\n\n" RESET);
	printf(GREEN "\n\nKnock, knock, Neo.\n\n" RESET);
	gen_matrix(array, width, height);
	print_matrix(array, buff, width, height);
	printf(GREEN "\n\nWake Up, Neo...\n\n" RESET);
	free(array);
	free(buff);
}

// Usage:
// matrix width height
int	matrix(char **argv)
{
	int		width;
	int		height;
	int		*array;
	char	*buff;

	buff = NULL;
	if (argv[1])
		width = ft_atoi(argv[1]);
	else
	{
		buff = malloc(1);
		width = (int)(uintptr_t)buff;
		width %= MATRIX;
		free(buff);
	}
	if (width == 0)
		width = MATRIX;
	array = malloc(width * sizeof(*array));
	buff = malloc(width + 1);
	if (argv[2])
		height = atoi(argv[2]);
	else
		height = MATRIX;
	matrix_rest(height, width, array, buff);
	return (0);
}
