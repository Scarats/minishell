#include "../../minishell.h"

// Will call the left node.
int left(t_main_data *data)
{
	close(data->node->pipefd[0]);
	if (data->node->pipefd[1] != STDOUT_FILENO)
		dup2(data->node->pipefd[1], STDOUT_FILENO);
	close(data->node->pipefd[1]);
	traverse_tree(data->node->left);
	exit(0);
}

// Will call the right node.
int right(t_main_data *data)
{
	close(data->node->pipefd[1]);
	if (data->node->pipefd[1] != STDOUT_FILENO)
		dup2(data->node->pipefd[1], STDOUT_FILENO);
	close(data->node->pipefd[0]);
	traverse_tree(data->node->right);
	exit(0);
}

// Will create two childs, left and right, for each end of the pipe.
int pipex(t_main_data *data)
{
	if (pipe(data->node->pipefd) == -1)
		return (perror("pipe"), 1);
	
	data->node->left->input_fd = data->node->input_fd;
    data->node->left->output_fd = data->node->pipefd[1];
    data->node->right->input_fd = data->node->pipefd[0];
    data->node->right->output_fd = data->node->output_fd;

	data->node->left_pid = fork();
	if (data->node->left_pid == -1 )
		return (perror("fork"), 1);
	else if (data->node->left_pid == 0)
		left(data->node);

	data->node->right_pid = fork();
	if (data->node->right_pid == -1)
		return (perror("fork"), 1);
	else if (data->node->right_pid == 0)
		right(data->node);

	close(data->node->pipefd[0]);
	close(data->node->pipefd[1]);
	waitpid(data->node->left_pid, NULL, 0);
	waitpid(data->node->right_pid, NULL, 0);
}
