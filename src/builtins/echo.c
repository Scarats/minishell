#include "../minishell.h"

void print_array(char **argv)
{
	int i;

	i = 0;
	while (argv[i])
		printf("%s", argv[i++]);
}

int echo(char **argv)
{
	bool n;

	n = false;
	if (argv[1] == NULL)
		return ((void)printf("\n"), 0);
	if (!ft_strncmp(argv[1], "-n", sizeof(argv[1])))
		n = true;
	if (n && !argv[2])
		return (printf("\n"));
	if (n)
		print_array(&argv[2]);
	else
	{
		print_array(&argv[1]);
		printf("\n");
	}
	return (0);
}
