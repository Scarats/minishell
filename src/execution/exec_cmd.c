#include "../../minishell.h"

// Check the redirections, change accordingly the inpout and output fds
int redirections(t_node *node, t_main_data *data)
{
	if (!node->cmd->redirection)
		return (0);
	else if (node->cmd->redirection == TOKEN_REDIRECT_IN) // <
	{
		// Try to open the file.
	}
	else if (node->cmd->redirection == TOKEN_REDIRECT_OUT) // >
	{
		// Will try to open the file or create it.
	}
	else if (node->cmd->redirection == TOKEN_HEREDOC) // <<
	{
		// Will wait for user to write text as input.
		// idk yet how to do it, so let's see later.
	}
	else if (node->cmd->redirection == TOKEN_APPEND) // >>
	{
		// Will write after the file
		// idk yet how to do it, so let's see later.
	}
}

// // Check permissions, if file exists etc...
// int checks(t_node *node, t_main_data *data)
// {
// 	if ()

// }

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
