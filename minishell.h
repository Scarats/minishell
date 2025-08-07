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

typedef enum e_token_type{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_OUT, // >
    TOKEN_REDIRECT_IN, // <
    TOKEN_APPEND, // >>
	TOKEN_HEREDOC, // <<
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF
} t_token_type;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR
} t_node_type;

typedef struct s_cmd
{
	char **tokens;	// Store the full command.
	
	t_token_type redirection;
	int error; // To catch excve errors.
} t_cmd;

typedef struct s_node
{
	struct s_node *left;
	struct s_node *right;
	struct s_node *parent; // Might be useless, let's see later.
	
	pid_t left_pid;
	pid_t right_pid;
	
	t_node_type type;
	t_cmd *cmd;

	int pipefd[2];
	int input_fd;
	int output_fd;
	
	char *filename;
} t_node;

typedef struct s_main_data
{
	t_node *node;
	t_list *malloc_list;
	
	int error;
	char *str_error;
} t_main_data;



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
int pipex(t_node *node, t_main_data *data);
int traverse_tree(t_node *node, t_main_data *data);
int exec_cmd(t_node *node, t_main_data *data);
int or_or(t_node *node, t_main_data *data);
int and_and(t_node *node, t_main_data *data);
int exec_cmd(t_node *node, t_main_data *data);

t_tokenizer *tokenizer_initializer(char *input);
t_token *checktoken(t_tokenizer *tok);
void ft_skipspace(t_tokenizer *tok);
t_token *token_and_or(t_tokenizer *tok);
t_token *token_append_heredoc(t_tokenizer *tok);
t_token *token_pipe(t_tokenizer *tok);
t_token *token_redirect_io(t_tokenizer *tok);
t_token *token_quote(t_tokenizer *tok);
t_token *token_word(t_tokenizer *tok);
t_token *get_next_token(t_tokenizer *tok);

t_cmd_node *command_node_init(void);
t_cmd_node *operator_node_init(t_node_type type, t_cmd_node *left, t_cmd_node *right);
void *argument(t_command *cmd, char *args);
char **args_to_array(t_command *cmd);

t_cmd_node* parse_command(t_tokenizer *tok);
t_cmd_node* parse_pipe(t_tokenizer *tok);
t_cmd_node* parse_and_or(t_tokenizer *tok);
t_cmd_node *parse_expression(t_tokenizer *tok);
t_cmd_node *parse_command_line(char *input);

#endif