#ifndef MINISHELL_H
#define MINISHELL_H

#include "./utils/libft/libft.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

typedef enum e_token_type
{
	TOKEN_CMD,			// ex: cat, sleep, ls ...
	TOKEN_ARGUMENT,		// ex: -a, -l, file.txt ... (a file without a redirection)
	TOKEN_FILE,			// ex: > file.txt, < file.txt ... (file with redirection)
	TOKEN_PIPE,			// |
	TOKEN_REDIRECT_OUT, // >
	TOKEN_REDIRECT_IN,	// <
	TOKEN_APPEND,		// >>
	TOKEN_HEREDOC,		// <<
	TOKEN_AND,			// &&
	TOKEN_OR,			// ||
	TOKEN_LPAREN,		// (
	TOKEN_RPAREN,		// )
	TOKEN_EOF
} t_token_type;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,	  // |
	NODE_AND,	  // &&
	NODE_OR,	  // ||
	NODE_SUBSHELL // ()
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
} t_output;

typedef struct s_cmd
{
	char **tokens; // Store the command for excve().

	t_token_type redirection;
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

	t_cmd *cmd;
	t_input *input;
	t_output *output;

	int exit_status;
} t_node;

typedef struct s_token
{
	t_token_type type;
	char *value;

	struct s_token *prev_token;
	struct s_token *next_token;
} t_token;

typedef struct s_tokenizer
{
	char *input;
	int pos;
	int prev_pos;
	int length;
	int depth;

	t_token_type curr_tok_type;

	t_token *token_list;
	int token_list_size; // Keep track of the number of tokens

	t_list *malloc_tok;
} t_tokenizer;

typedef struct s_main_data
{
	t_node *node;
	t_list *malloc_list;

	char *input;
	char **argv;
	int argc;

	int error;
	char *str_error;

	int last_exit_status;

	t_tokenizer *tok;
} t_main_data;

#endif