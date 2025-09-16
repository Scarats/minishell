#include "../minishell.h"

int exec_builtins(t_node *node, t_main_data *data)
{
	int error;
	int lenght;

	if (!data)
		data = NULL;

	lenght = ft_strlen(node->cmd_argv[0]);

	error = 0;
	if (ft_strncmp(node->cmd_argv[0], "cd", lenght))
	{
		error = cmd_cd();
	}
	return (error);
}