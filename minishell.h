#ifndef MINISHELL_H
#define MINISHELL_H

#include "./utils/libft/libft.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR
} t_node_type;

typedef struct s_input
{
	char *filename;
	int fd;
} t_input;

typedef struct s_output
{
	char *filename;
	int fd;
	bool append; // for >>
} t_output;

typedef struct s_cmd
{
	char **tokens;	// Store the full command,but is a duplicata since below is the same.
	char *path; // Can be a file or a directory (i guess lol)

	int error; // To catch excve errors.
} t_cmd;

typedef struct s_node
{
	t_node_type type;

	struct s_node *left;
	struct s_node *right;
	struct s_node *parent; // Might be useless, let's see later.

	t_cmd *cmd;
	t_input *input;
	t_output *output;
} t_node;

typedef struct s_main_data
{
	t_node *node;
	t_list *malloc_list;
} t_main_data;

#endif