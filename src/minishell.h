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
#include <dirent.h>
#include <stdint.h>
#include <limits.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define RED "\x1B[91m"	  // Lighter red
#define ORANGE "\x1B[33m" // Orange/yellow
#define GREEN "\x1B[32m"  // Green
#define BLUE "\x1B[94m"	  // Light blue
#define BROWN "\x1B[31m"  // Approximation using red (no true brown in ANSI)
#define PURPLE "\x1B[35m" // Magenta (common substitute for purple)
#define RESET "\x1B[0m"
#define MATRIX 100

extern volatile sig_atomic_t stop_flag; // declaration
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
	NODE_PIPE, // |
	NODE_AND,  // &&
	NODE_OR,   // ||
} t_node_type;

typedef struct s_env
{
	char *name;
	char *value;

	bool exported;

	struct s_env *next;
} t_env;

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
	bool builtin;
	bool create_subshell; // Trigger a subshell creation.
	int in_subshell;	  // increase each time we create a subshell, it's the depth of subshells.

	struct s_node *left;
	struct s_node *right;

	int pipefd[2];
	pid_t left_pid;
	pid_t right_pid;

	t_env *curr_env;
	char **cmd_argv;
	t_redir *redirection;
	char *path;

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

	bool in_child;

	void *root; // Pointer to root, has to be casted at the beginning.
	// Copy of the root env, to be passed to execve, it contains the local var of this command.
	t_env *curr_env;

	t_tokenizer *tok;
} t_main_data;

typedef struct s_root
{
	t_main_data *data;

	int last_exit_status;

	t_env *env;

	t_list *malloc_root;
} t_root;

t_token_type get_tok_type(char c, char next);
char check_next_char(char *str, int pos);
t_token *add_to_list(t_main_data *data, t_token *prev);
int parser(t_main_data *data);
int traverse_tree(t_node *node, t_main_data *data);
int handle_quotes(t_main_data *data);
char *clean_string(char *input);
t_node_type map_token_to_node(t_token_type t);
int is_redir(t_token_type type);
t_redir *add_redirection(t_main_data *data, t_node *node, t_token_type type);
int is_and_or(t_token_type t);
t_node *build_tree(t_main_data *data, t_token *tok_list, int size, int depth);
int get_cmd_argc(t_token *tok_list, int size);
int check_paren_error(t_token *tok_list, int size);
int wrapped_in_paren(t_token *tok_list, int size);
int traverse_tree(t_node *node, t_main_data *data);
int exec_cmd(t_node *node, t_main_data *data);
int and_and(t_node *node, t_main_data *data);
int or_or(t_node *node, t_main_data *data);
int pipes(t_node *node, t_main_data *data);
int exec_builtins(t_node *node, t_main_data *data);
int is_builtin(char *cmd);
int cd(t_node *node);
int pwd(void);
int echo(char **argv);
int matrix(char **arg);
t_env *copy_env(t_list **malloc_list, t_env *env);
t_env *set_env_var_list(t_list **malloc_list, char **env);
char *get_env_var(t_env *env, char *target);
int export(t_root *root, char *var);
int env(t_env *root_env);

#endif