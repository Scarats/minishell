#include "../minishell.h"

// Execute built-in functions.
int exec_builtins(t_node *node, t_main_data *data)
{
	int error;
	int length;

	if (!data)
		data = NULL;

	length = ft_strlen(node->cmd_argv[0]);

	error = 0;
	if (!ft_strncmp(node->cmd_argv[0], "cd", length))
		error = cd(node);
	else if (!ft_strncmp(node->cmd_argv[0], "pwd", length))
		error = pwd();
	else if (!ft_strncmp(node->cmd_argv[0], "echo", length))
		error = echo(node->cmd_argv);
	else if (!ft_strncmp(node->cmd_argv[0], "matrix", length))
		error = matrix(node->cmd_argv);
	else
		printf(RED"error: built-in not found.\n"RESET);
	return (error);
}