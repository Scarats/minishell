#include "../minishell.h"
 
int cd(t_node *node)
{
	int error;

	error = 0;
	if (arraylen(node->cmd_argv) > 2)
		return (printf(RED"error: too many arguments.\n"RESET), 1);	
	if (error)
		return (error);
	error = chdir(node->cmd_argv[1]);
	if (error)
		return (fdprintf(2, "cd: %s: %s\n", strerror(errno), node->cmd_argv[1]), 1);
	return (error);
}