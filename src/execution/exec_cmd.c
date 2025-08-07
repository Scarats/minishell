#include "../../minishell.h"

// Check if binary exist and if user has access.
int binaries_check(t_node *node, t_main_data *data)
{

}

// Check the redirections, change accordingly the inpout and output fds
// If redirected, changes the fd.
int redirections(t_node *node, t_main_data *data)
{
	if (!node->cmd->redirection)
		return (0);
	else if (node->cmd->redirection == TOKEN_REDIRECT_IN) // <
	{
		// Try to open the file.
		node->input_fd = open(node->filename, O_RDONLY);
		if (node->input_fd < 0)
			return (1);
	}
	else if (node->cmd->redirection == TOKEN_REDIRECT_OUT) // >
	{
		// Will try to open the file or create it.
		node->output_fd = open(node->filename, O_WRONLY | O_CREAT | O_TRUNC);
		if (node->output_fd < 0)
			return (1);
	}
	// else if (node->cmd->redirection == TOKEN_HEREDOC) // <<
	// {
	// 	// Will wait for user to write text as input.
	// 	// idk yet how to do it, so let's see later.
	// }
	// else if (node->cmd->redirection == TOKEN_APPEND) // >>
	// {
	// 	// Will write after the file
	// 	// idk yet how to do it, so let's see later.
	// }
	return (0);
}

// Check permissions for file, according to the type of redirection and command.
int file_checks(t_node *node, t_main_data *data)
{
	// first check the bin access.
	if (check_access())

}

// Execute the command.
int execution(t_node *node, t_main_data *data)
{

}

// Handle the execution process.
// Should handle the bin before creating and opening the files.
int exec_cmd(t_node *node, t_main_data *data)
{
	int pid;
	int error;

	error = 0;
	pid = fork();
	if (pid == -1)
		return (1);
	else if (pid == 0)
	{
		error = binaries_check(node, data);
		if (error == 0)
			error = redirections(node, data);
		if (error == 0)
			error = file_check(node, data);
		if (error == 0)
			error = execution(node, data);
	}
	if (node->input_fd)
		close(node->input_fd);
	if (node->input_fd)
		close(node->input_fd);
	waitpid(pid, NULL, 0);
	if (error != 0)
		return (error);
	return (0);
}
