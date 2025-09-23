#include "../minishell.h"

// Execute built-in functions.
int exec_builtins(t_node *node, t_main_data *data)
{
	int error;
	int lenght;
	t_root *root;

	if (!data)
		data = NULL;

	root = data->root;
	lenght = ft_strlen(node->cmd_argv[0]);

	error = 0;
	if (!ft_strncmp(node->cmd_argv[0], "cd", lenght))
		error = cd(node);
	else if (!ft_strncmp(node->cmd_argv[0], "pwd", lenght))
		error = pwd();
	else if (!ft_strncmp(node->cmd_argv[0], "echo", lenght))
		error = echo(node->cmd_argv);
	else if (!ft_strncmp(node->cmd_argv[0], "matrix", lenght))
		error = matrix(node->cmd_argv);
	else if (!ft_strncmp(node->cmd_argv[0], "export", lenght))
		error = export(root, node->cmd_argv[1]);
	else if (!ft_strncmp(node->cmd_argv[0], "env", lenght))
		error = env(root->env);
	else
		printf(RED "error: built-in not found.\n" RESET);
	return (error);
}