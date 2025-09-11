#ifndef MINISHELL_H
#define MINISHELL_H

#include "../utils/libft/libft.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

// typedef enum e_quote_state {
//     QUOTE_NONE,
//     QUOTE_SINGLE,
//     QUOTE_DOUBLE
// } t_quote_state;

typedef enum e_char_type
{
	CHAR_SPACE,
	CHAR_TEXT,
	CHAR_OPERATOR,
	CHAR_PARENTHESIS,
	CHAR_SINGLE_QUOTE,
	CHAR_DOUBLE_QUOTE,
	CHAR_NULL
} t_char_type;

// Used for tokens but also during the token creation to type each char.
typedef enum e_token_type
{
	TOKEN_ERROR,
	TOKEN_CMD,			// ex: cat, sleep, ls ...
	TOKEN_ARGUMENT,		// ex: -a, -l, file.txt ... (a file without a redirection)
	TOKEN_FILE,			// ex: > file.txt, < file.txt ... (file with redirection)
	TOKEN_PIPE,			// |
	TOKEN_REDIRECT_OUT, // >
	TOKEN_REDIRECT_IN,	// <
	TOKEN_APPEND,		// >>
	TOKEN_HEREDOC,		// <<
	TOKEN_AND_AND,		// &&
	TOKEN_AND,			// &
	TOKEN_OR,			// ||
	TOKEN_LPAREN,		// (
	TOKEN_RPAREN,		// )
	TOKEN_SPACE,		// " "
	TOKEN_NULL,			// \0
	TOKEN_DOLLAR,		// $
	TOKEN_ENV_VAR,		//$variable
	TOKEN_TEXT,
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

typedef struct s_redir
{
	t_token_type type;
	char *filename;
	int fd;
	struct s_redir *next;
} t_redir;

typedef struct s_node
{
	t_node_type type;
	bool subshell;

	struct s_node *left;
	struct s_node *right;

	int pipefd[2];
	pid_t left_pid;
	pid_t right_pid;

	char **cmd_argv;
	t_redir *redirection;

	int input_fd;
	int output_fd;

	int exit_status;
} t_node;

typedef struct s_token
{
	t_token_type type;
	char *word;

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
	bool in_word;

	char next_char;

	t_token_type curr_tok_type;
	t_token_type prev_type;

	t_char_type curr_char_type;
	t_char_type prev_char_type;

	bool double_quote;
	bool single_quote;

	t_token *token_list;
	t_token *last_token;

	t_token *token_array;

	int token_list_size; // Keep track of the number of tokens
} t_tokenizer;

typedef struct s_main_data
{
	t_node *node;
	t_list *malloc_tok;
	t_list *malloc_tree;

	char *input;
	char **argv;
	int argc;

	int error;
	char *str_error;

	int last_exit_status;

	t_tokenizer *tok;
} t_main_data;

t_token_type get_tok_type(char c, char next);
char check_next_char(char *str, int pos);
t_token *add_to_list(t_main_data *data, t_token *prev, int start, int end);
int parser(t_main_data *data);
int traverse_tree(t_node *node, t_main_data *data);
int handle_quotes(t_main_data *data);
char *clean_string(char *input);
t_node_type map_token_to_node(t_token_type t);
int	is_redir(t_token_type type);
t_redir	*add_redirection(t_main_data *data, t_node *node, t_token_type type);
int	is_and_or(t_token_type t);
t_node *build_tree(t_main_data *data, t_token *tok_list, int size);
int get_cmd_argc(t_token *tok_list, int size);
int check_paren_error(t_token *tok_list, int size);
int wrapped_in_paren(t_token *tok_list, int size);

#endif