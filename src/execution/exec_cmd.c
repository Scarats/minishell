#include "../minishell.h"

// Check if binary exist and if user has access.
int binaries_check(t_node *node, t_main_data *data)
{

}

// Open with accrding flags to action number.
int open_file(char *filename, int action)
{
	int fd;

	fd = -1;
	if (action == 1)
		fd = open(filename, O_RDONLY);
	else if (action == 2)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
	else if (action == 3)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND);

	if (fd < 0)
		return (1); // Error.
	if (action == 1 && dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), 1); // Error.
	else if (action > 1 && dup2(fd, STDOUT_FILENO) == -1)
		return (close(fd), 1);
	close(fd);
	return (0);
}

// Check the redirections, change accordingly the inpout and output fds
// If redirected, changes the fd.
int redirections(t_node *node, t_main_data *data)
{
	t_redir *redir;

	redir = node->redirection;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRECT_IN && open_file(redir->filename, 1))
			return (1);
		else if (redir->type == TOKEN_REDIRECT_OUT && open_file(redir->filename, 2))
			return (1);
		else if (redir->type == TOKEN_APPEND && open_file(redir->filename, 3))
			return (1);
		redir = redir->next;
	}
	return (0);
}

// Execute the command.
int execution(t_node *node, t_main_data *data)
{

}

int set_io_fds(t_node *node, t_main_data *data)
{
	if (node->input_fd != -1 && node->input_fd != STDIN_FILENO)
	{
    	dup2(node->input_fd, STDIN_FILENO);
		close(node->input_fd);
	}
	if (node->output_fd != -1 && node->output_fd != STDOUT_FILENO)
	{	
		dup2(node->output_fd, STDOUT_FILENO);
		close(node->output_fd);
	}
	return (0);
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
			error = set_io_fds(node, data);
		if (error == 0)
			error = redirections(node, data);
		if (error == 0)
			error = execution(node, data);
	}
	waitpid(pid, NULL, 0);
	return (error);
}
