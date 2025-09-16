#include "../minishell.h"


// In an array of size x, each number has a value from -size to size.
// I prints only when positive, and reset to -size when reach size.
// It will print random chars, also from unicode.
// Neighbors cannot print at the same time, so their difference has to be at least > time.

// Read 
int gen_random(int size)
{
	int fd;
	int seed;

	seed = 0;
	fd = open("/dev/urandom", O_RDONLY);
	if (!fd)
		fd = open("/dev/random", O_RDONLY);
	if (!fd)
		seed = (int)&seed;
	else
	{
		read(fd, &seed, sizeof(seed));
		close(fd);
	}
	return((seed % size + 1) - size);
}

void gen_matrix(int *array, int size)
{
	int i;
	int prev;

	i = 1;
	array[0] = gen_random(size);	
	prev = array[0];
	while (i < 30)
	{
		array[i] = gen_random(size - prev); // Or prev - size
		prev = array[i];
		i++;
	}
}

void fill_buff(int *array, char *buff, int size)
{
	int i;

	i = 0;
	while (i < 30)
	{
		if (array[i] > 0)
			buff[i] = ((126 - 33) % gen_random(size)) + 33;
		else
			buff[i] = ' ';
		if (array[i] <= size)
			array[i] += 1;
		else
			array[i] = size;
		i++;
	}
}

void print_matrix(int *array, char *buff, int size)
{
	while (1)
	{
		fill_buff(array, buff, size);

	}

}

// Kill it with signal
int matrix(char *arg)
{
	int size;
	int *array[30];
	char *buff[30];

	size = ft_atoi(arg);
	gen_matrix(array, size);
	print_matrix(array, buff, size);
}