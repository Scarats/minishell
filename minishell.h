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
    TOKEN_WORD,
    TOKEN_PIPE, // |
    TOKEN_REDIRECT_OUT, // >
    TOKEN_REDIRECT_IN, // <
    TOKEN_APPEND, // >>
	TOKEN_HEREDOC, // <<
    TOKEN_AND, // &&
    TOKEN_OR, // ||
    TOKEN_LPAREN, // (
    TOKEN_RPAREN, // )
    TOKEN_EOF
} t_token_type;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE, // |
	NODE_AND, // &&
	NODE_OR, // ||
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
	char **tokens;	// Store the command for excve().
	
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

	s_token *prev_token;
	s_token *next_token;
} t_token;

typedef struct s_tokenizer
{
    char *input;
    int pos;
    int length;
	int depth;
	t_token *token_list;
} t_tokenizer;

// typedef struct s_arg_node
// {
//     char *value;
//     struct s_arg_node *next;
// } t_arg_node;

// typedef struct s_command
// {
// 	char **argv; // node->input->args; this one is for each node
// 	char *input;; // data->input; this one is for the main_data struct
// 	// t_arg_node *args_first;
// 	// t_arg_node *args_last;
// 	int argc; // data->argc;
// 	char *file_in; // = node->input->filename;
// 	char *file_out; // = node->output->filename;
// 	// int append_out;
// 	int exit_status; // = node->exit_status; 
// 	int final_exit_status; // data->final_exit_status; for the main_data struct
// } t_command;

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


// // FUNCTIONS
// int pipex(t_node *node, t_main_data *data);
// int traverse_tree(t_node *node, t_main_data *data);
// int exec_cmd(t_node *node, t_main_data *data);
// int or_or(t_node *node, t_main_data *data);
// int and_and(t_node *node, t_main_data *data);
// int exec_cmd(t_node *node, t_main_data *data);

// int tokenizer_initializer(t_main_data *data);
// t_token *checktoken(t_tokenizer *tok);
// void ft_skipspace(t_tokenizer *tok);
// t_token *token_and_or(t_tokenizer *tok);
// t_token *token_append_heredoc(t_tokenizer *tok);
// t_token *token_pipe(t_tokenizer *tok);
// t_token *token_redirect_io(t_tokenizer *tok);
// t_token *token_quote(t_tokenizer *tok);
// t_token *token_word(t_tokenizer *tok);
// t_token *get_next_token(t_tokenizer *tok);

// t_cmd_node *command_node_init(void);
// t_cmd_node *operator_node_init(t_node_type type, t_cmd_node *left, t_cmd_node *right);
// void add_argument(t_command *cmd, char *args);
// char **args_to_array(t_command *cmd);

// t_cmd_node* parse_command(t_tokenizer *tok);
// t_cmd_node* parse_pipe(t_tokenizer *tok);
// t_cmd_node* parse_and_or(t_tokenizer *tok);
// int	parse_expression(t_main_data *data);
// t_node *parse_command_line(t_main_data *data);

#endif