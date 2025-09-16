#include "../minishell.h"

int echo(char **argv)
{
	bool n;

	n = false;
	if (!ft_strncmp(argv[1], "-n", sizeof(argv[1])))
		n = true;
	if (n)
		printf("%s", argv[2]);
	else
	{
		if (argv[1])
			printf("%s", argv[1]);
		printf("\n");
	}
	return (0);
}