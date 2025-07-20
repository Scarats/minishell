#ifndef PIPEX_H
# define PIPEX_H

# include "./libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipe
{
	int fd[2];    // For the pipe
	pid_t pid;    // For the forks
	char *buff;   // To store the output of cmd1
	char **args1; // For execve
	char **args2; // For execve

	t_list *lst; // For malloc

	char	**av;
	char	*file1;
	char	*file2;
	char	*cmd1;
	char	*cmd2;
	int		file1fd;
	int		file2fd;
}			t_pipe;

#endif