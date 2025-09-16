#include "../minishell.h"

// Read 
int gen_random(int size)
{
	int fd;
	int seed;
	char *addr;

	addr = NULL;
	seed = 0;
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		fd = open("/dev/random", O_RDONLY);
	if (fd == -1)
	{
		addr = malloc(1);	
		seed = ft_atoi((char *)&addr);
		free(addr);
	}
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
	int range;

	i = 1;
	array[0] = gen_random(size);	
	prev = array[0];
	while (i < 30)
	{
		if ((size - prev) <= 0)
			range = prev - size;
		else
			range = size - prev;
		array[i] = gen_random(range);
		prev = array[i];
		i++;
	}
}

void fill_buff(int *array, char *buff, int size)
{
    int i;
    int r;
    int ch;
    const int printable_count = 126 - 33 + 1;

    for (i = 0; i < MATRIX; ++i)
    {
        if (array[i] > 0)
        {
            r = gen_random(size);
            if (r < 0)
                r = -r;
            if (r == 0)
                r = 1;
            ch = 33 + (r % printable_count);
            buff[i] = (char)ch;
        }
        else
            buff[i] = ' ';
        if (array[i] >= size)
            array[i] = -size;
        else
            array[i] += 1;
    }
    buff[MATRIX] = '\0';
}

void print_matrix(int *array, char *buff, int size)
{
	while (1)
	{
		fill_buff(array, buff, size);
		printf(GREEN"%s\n"RESET, buff);
	}

}

// Kill it with signal
int matrix(char *arg)
{
    int size;
    int array[MATRIX];     // array of ints, not pointers
    char buff[MATRIX + 1];     // buffer with room for null terminator
	char *addr;

	addr = NULL;
	printf(RED"HOOOLLLLA\n"RESET);
	if (arg)
		size = ft_atoi(arg);
	else
	{
		addr = malloc(1);
		size = ft_atoi((char *)&addr);
		free(addr);
	}
	gen_matrix(array, size);
	print_matrix(array, buff, size);
	return (0);
}