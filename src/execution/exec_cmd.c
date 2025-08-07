#include "../../minishell.h"

// Check the redirections, change accordingly the inpout and output fds
int redirections(node, data)
{

}

// Check permissions, if file exists etc...
int checks(t_node *node, t_main_data *data)
{

}

// Execute the command.
int execution(t_node *node, t_main_data *data)
{
}

// Handle the execution process.
int exec_cmd(t_node *node, t_main_data *data)
{
	int pid;

	pid = fork();
	if (pid == -1)
		return (1);
	else if (pid == 0)
	{
		if (redirections(node, data))
			return (1);
		else if (checks(node, data))
			return (1);
		else if (execution(node, data))
			return (1);
	}
	waitpid(pid, NULL, 0);
	return (0);
}
