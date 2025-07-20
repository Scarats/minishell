/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 19:05:05 by tcardair          #+#    #+#             */
/*   Updated: 2025/07/20 19:18:03 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void child(t_pipe *data)
{
    close(data->fd[0]);  // Close read end of pipe
    
    // Redirect input file to stdin
    dup2(data->file1fd, STDIN_FILENO);
    close(data->file1fd);
    
    // Redirect stdout to pipe write end
    dup2(data->fd[1], STDOUT_FILENO);
    close(data->fd[1]);
    
    execve(data->cmd1, data->args1, NULL);
    perror("execve child");
    exit(EXIT_FAILURE);
}

void parent(t_pipe *data)
{
    // Close unused write end of the pipe
    close(data->fd[1]);

    // Redirect stdin to the pipe's read end
    dup2(data->fd[0], STDIN_FILENO);

    // Redirect stdout to the outfile
    dup2(data->file2fd, STDOUT_FILENO);

    // Close the original read end and output file fd (they've been duplicated)
    close(data->fd[0]);
    close(data->file2fd);

    // Execute the second command
    execve(data->cmd2, data->args2, NULL);

    // If execve fails
    perror("execve parent");
    exit(EXIT_FAILURE);
}


// Get the cmd1/2 and their attributes.
int parser(t_pipe *data)
{
    // Split commands
    data->args1 = ft_split(data->av[2], ' ');
    if (!data->args1)
        return (perror("ft_split failed for cmd1"), 1);

	my_array_addtolist(&data->lst, (void **)data->args1);

    data->args2 = ft_split(data->av[3], ' ');
    if (!data->args2)
        return (perror("ft_split failed for cmd2"), 1);

	my_array_addtolist(&data->lst, (void **)data->args2);

	// find the binaries
	data->cmd1 = find_bin(data->args1[0]);
	data->cmd2 = find_bin(data->args2[0]);

    if (!data->cmd1)
        return (write (2, data->args1[0], ft_strlen(data->args1[0])), perror(" not found"), 1);
    if (!data->cmd2)
        return (write (2, data->args2[0], ft_strlen(data->args2[0])), perror(" not found"), 1);

	my_addtolist(&data->lst, data->cmd1);
	my_addtolist(&data->lst, data->cmd2);
	
    // Rest of your code...
    return (0);
}

// Returns 0 for success and 1 for failure.
int check_input(int ac, t_pipe *data)
{
	if (ac != 5)
		return (perror("invalid input"), 1);

	if (parser(data))
		return (1);

	// Add the files names to the structure for clarity
	data->file1 = data->av[1];
	data->file2 = data->av[4];

	// open files
	data->file1fd = open(data->file1, O_RDONLY);
	data->file2fd = open(data->file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->file1fd < 0)
		return (perror("failed to open file1"), 1);
	if (data->file2fd < 0)
		return (perror("failed to open file2"), close(data->file1fd), 1);

	// Check file access.
	if (!check_access(data->file1, 1, 1, 0, 0) && !check_access(data->cmd1, 1, 0, 1, 0) && !check_access(data->cmd2, 1, 0, 1, 0) && !check_access(data->file2, 1, 1, 0, 0))
		return (0);
	else
		return (1);
}

int	main(int ac, char **av)
{
	t_pipe data;

	ft_memset(&data, 0, sizeof(data));
	data.av = av;

	// Check input and parsing.
	if (check_input(ac, &data))
		return (my_free(&data.lst), 2);
	data.buff = my_malloc(&data.lst, ft_strlen(av[1]) + 1);
	
	// Create the pipe and check for error.
	if (pipe(data.fd) == -1)
		return (perror("pipe"), free(data.buff), exit(EXIT_FAILURE), 2);
	
	// Fork
	data.pid = fork();
	
	// Check error.
	if (data.pid == -1)
		return (perror("fork"), free(data.buff), exit(EXIT_FAILURE), 2);

	// Handle the child and parents logics
	else if (data.pid == 0)
		child(&data);
	else
		parent(&data);
	wait(NULL);
	return (my_free(&data.lst), close(data.file1fd), close(data.file2fd), EXIT_SUCCESS);
}
