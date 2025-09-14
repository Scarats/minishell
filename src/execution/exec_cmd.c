#include "../minishell.h"

// Check if binary exist and if user has access.
int get_bin_path(t_node *node, t_main_data *data)
{
	if (!data)
		data = NULL;
	
	node->path = find_bin(node->cmd_argv[0]);
	if (node->path)
		return (0);
	return (1);
}

// Open with accrding flags to action number.
int open_file(char *filename, int action)
{
	int fd;

	fd = -1;
	if (action == 1)
		fd = open(filename, O_RDONLY);
	else if (action == 2)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (action == 3)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);

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
	if (!data)
		data = NULL;

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
// Should not return since the program will be replaced by execve.
int execution(t_node *node, t_main_data *data)
{
	if (!data)
		data = NULL;

	execve(node->path, node->cmd_argv, NULL);
	return (1);
}

int set_io_fds(t_node *node, t_main_data *data)
{
	if (!data)
		data = NULL;


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

// Call step by step each function for clean execution.
int child_exec(t_main_data *data, t_node *node)
{
	int error;

	error = 0;
	error = get_bin_path(node, data);
	if (error == 0)
		error = set_io_fds(node, data);
	if (error == 0)
		error = redirections(node, data);
	if (error == 0)
	{
		execution(node, data);
		error = 1; // Should not arrive here.
	}
	return (error);
}

// Handle the execution process.
// Should handle the bin before creating and opening the files.
int exec_cmd(t_node *node, t_main_data *data)
{
	int pid;

	pid = -1;
	if (data->in_child)
		return (child_exec(data, node));
	
	pid = fork();
	if (pid == -1)
		return (1);
	
	if (pid == 0)
		child_exec(data, node);
	return (waitpid(pid, NULL, 0) == -1);
}
