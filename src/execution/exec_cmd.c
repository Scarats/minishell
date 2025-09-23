#include "../minishell.h"

// Check if binary exist and if user has access.
int get_bin_path(t_node *node, t_main_data *data)
{
	if (!data)
		data = NULL;

	if (!node->cmd_argv || !node->cmd_argv[0])
		return (1);
	if (ft_strchr(node->cmd_argv[0], '/'))
	{
		if (access(node->cmd_argv[0], X_OK) == 0)
		{
			node->path = node->cmd_argv[0];
			return (0);
		}
		return (1);
	}
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
	if (node->builtin)
		return (exec_builtins(node, data));
	else
		execve(node->path, node->cmd_argv, NULL);
	return (1);
}

int set_io_fds(t_node *node, t_main_data *data)
{
	if (!data)
		data = NULL;

	if (node->input_fd != -1)
	{
		dup2(node->input_fd, STDIN_FILENO);
		close(node->input_fd);
	}
	if (node->output_fd != -1)
	{
		dup2(node->output_fd, STDOUT_FILENO);
		close(node->output_fd);
	}
	return (0);
}

// Call step by step each function for clean execution.
int exec_handler(t_main_data *data, t_node *node)
{
	int error;

	error = 0;
	if (!node->builtin)
		error = get_bin_path(node, data);
	if (error == 0)
		error = set_io_fds(node, data);
	if (error == 0)
		error = redirections(node, data);
	if (error == 0)
		error = execution(node, data);
	return (error);
}

// Run builtin in parent: save fds, apply redirs, run, then restore.
int exec_builtin_in_parent(t_node *node, t_main_data *data)
{
    int saved_in;
    int saved_out;
    int error;

    saved_in = dup(STDIN_FILENO);
    saved_out = dup(STDOUT_FILENO);
    if (saved_in == -1 || saved_out == -1)
    {
        if (saved_in != -1)
			close(saved_in);
        if (saved_out != -1)
			close(saved_out);
        return (1);
    }

    error = exec_handler(data, node);

    // Restore stdio no matter what
    if (dup2(saved_in, STDIN_FILENO) == -1)
		error = 1;
    if (dup2(saved_out, STDOUT_FILENO) == -1)
		error = 1;
    close(saved_in);
    close(saved_out);
    return (error);
}

// Handle the execution process.
// Should handle the bin before creating and opening the files.
int exec_cmd(t_node *node, t_main_data *data)
{
    int pid;
    int status;
    int error;

    if (node->builtin)
    {
        // If already in a child (e.g., inside a pipe/subshell), just run it here.
        if (data->in_child)
            return (exec_handler(data, node));
        // Otherwise run in parent but restore stdio after redirections.
        return (exec_builtin_in_parent(node, data));
    }
    // If you have parent-only builtins, handle and return here:
    // if (!data->in_child && is_parent_builtin(node)) return run_builtin_in_parent(node, data);

    pid = fork();
    if (pid == -1)
        return 1;

    if (pid == 0)
    {
        error = exec_handler(data, node); // sets pipe defaults, applies redirs, then execve
        exit(error);
    }

    if (node->input_fd != -1 && node->input_fd != STDIN_FILENO)
        close(node->input_fd);
    if (node->output_fd != -1 && node->output_fd != STDOUT_FILENO)
        close(node->output_fd);

    if (waitpid(pid, &status, 0) == -1)
        return (1);

    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return (1);
}
