#include "../minishell.h"
 
int cd(t_node *node)
{
	int error;

	error = 0;
	if (arraylen(node->cmd_argv) > 2)
		return (fdprintf(2, "error: too many arguments.\n"), 1);
	
	if (error)
		return (error);
	if (node->cmd_argv[1] == NULL)
		return (1);
	error = chdir(node->cmd_argv[1]);
	if (error)
		return (fdprintf(2, "minishell: cd: %s: %s\n", node->cmd_argv[1], strerror(errno)), 1);
	return (error);
}