#include "../minishell.h"

void print_array(char **argv)
{
	int i;

	i = 0;
	while (argv[i])
	{
		if (i > 0)
			ft_printf(" ");
		ft_printf("%s", argv[i++]);
	}
}

int is_n(char *str)
{
	int i;

	i = 1;
	if (!str[0] || !str[1])
		return (0);
	if (str[0] == '-' && str[1] == 'n')
	{
		while(str[++i])
		{
			if (str[i] != 'n')
				return (0);
		}
	}
	else
		return (0);
	return (1);
}

int echo(char **argv)
{
	bool n;

	n = false;
	if (argv[0] == NULL)
		return ((void)printf("\n"), 0);
	if (is_n(argv[0]))
		n = true;
	if (n && !argv[1])
		return (ft_printf("\n"));
	if (n)
		print_array(&argv[1]);
	else
	{
		print_array(&argv[0]);
	}
	return (0);
}
