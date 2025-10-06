#include "../minishell.h"

int create_subshell(t_node *node, t_main_data *data)
{
	int pid;
	int status;
	int error;

	error = 0;
	status = 0;
	pid = -1;
	printf(RED"SUBSHELL\n"RESET);
	pid = fork();
	if (pid < 0)
		return (1);
	else if (pid == 0)
	{
		node->create_subshell = false;
		error = traverse_tree(node, data);
		my_multi_free(&data->root->list_of_list);
		exit(error);
	}
	if (waitpid(pid, &status, 0) == -1)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

// Will check the type of the node.
// Then redirect to the according type function.
// These functions will execute recursively by calling traverse_tree again.
// The recursion breakpoint is reached when a NODE_COMMAND is reached.
int traverse_tree(t_node *node, t_main_data *data)
{
	printf("\n");
	for (int i = 0; node->cmd_argv[i]; i++)
		ft_printf(GREEN"TRAVERSE_TREE: %s\n"RESET, node->cmd_argv[i]);
	printf("\n");
	if (!node || !data)
		return (1);
	// Create a subshell here.
	else if (node->create_subshell)
		return (create_subshell(node, data));
	else if (node->type == NODE_COMMAND)
		return (exec_cmd(node, data));
	else if (node->type == NODE_PIPE)
		return (pipes(node, data));
	else if (node->type == NODE_AND)
		return (and_and(node, data));
	else if (node->type == NODE_OR)
		return (or_or(node, data));
	return (1); // Should not arrive here i guess
}
