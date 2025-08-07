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
	char **tokens;	// Store the full command.

	int error; // To catch excve errors.
} t_cmd;

typedef struct s_node
{
	t_node_type type;

	struct s_node *left;
	struct s_node *right;
	struct s_node *parent; // Might be useless, let's see later.

	int pipefd[2];
	pid_t left_pid;
	pid_t right_pid;

	int input_fd;
	int output_fd;

	t_cmd *cmd;
	t_input *input;
	t_output *output;
} t_node;

typedef struct s_main_data
{
	t_node *node;
	t_list *malloc_list;
} t_main_data;

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_IN,
    TOKEN_APPEND,
	TOKEN_HEREDOC,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF
} t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *value;
} t_token;

typedef struct s_tokenizer
{
    char *input;
    int pos;
    int length;
} t_tokenizer;

typedef struct s_arg_node {
    char *value;
    struct s_arg_node *next;
} t_arg_node;

typedef struct s_command
{
	char **args;
	t_arg_node *args_first;
	t_arg_node *args_last;
	int argc;
	char *file_in;
	char *file_out;
	int append_out;
	int exit_status;
} t_command;

typedef struct s_cmd_node
{
	t_node_type type;
	union {
		t_command *command;
		struct {
			struct s_cmd_node *left;
			struct s_cmd_node *right;
		};
	};
	int exit_status;
} t_cmd_node;

// FUNCTIONS
int pipex(t_node *node);
int traverse_tree(t_node *node);

#endif