#include "../minishell.h"

// Execute built-in functions.
int exec_builtins(t_node *node, t_main_data *data)
{
	int error;
	int lenght;

	if (!data)
		data = NULL;
	if (!node)
		return (1);

	lenght = ft_strlen(node->cmd_argv[0]);
	for (int i = 0; node->cmd_argv[i]; i++)
		ft_printf(GREEN"EXEC_BUILDINS %i: %s\n"RESET, i, node->cmd_argv[i]);
	error = 0;
	if (!ft_strncmp(node->cmd_argv[0], "cd", lenght))
		error = cd(node);
	else if (!ft_strncmp(node->cmd_argv[0], "pwd", lenght))
		error = pwd();
	else if (!ft_strncmp(node->cmd_argv[0], "echo", lenght))
		error = echo(&node->cmd_argv[1]);
	else if (!ft_strncmp(node->cmd_argv[0], "matrix", lenght))
		error = matrix(node->cmd_argv);
	else if (!ft_strncmp(node->cmd_argv[0], "export", lenght))
		error = export(data->root, node->cmd_argv[1]);
	else if (!ft_strncmp(node->cmd_argv[0], "env", lenght))
		error = env(data->root->env);
	else if (!ft_strncmp(node->cmd_argv[0], "unset", lenght))
		error = unset(&data->root->env, &node->cmd_argv[1]);
	else
		printf(RED "error: built-in not found.\n" RESET);
	return (error);
}
