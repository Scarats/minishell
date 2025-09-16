#include "../minishell.h"
 
int cmd_cd(t_node *node)
{
	int error;

	error = 0;
	if (arraylen(node->cmd_argv) > 2)
		return (printf(RED"error: too many arguments.\n"RESET), 1);
	error = chdir(node->cmd_argv[1]);
	if (error != 0)
		return (1);
	return (0);
}