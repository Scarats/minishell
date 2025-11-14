/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:08:53 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/14 19:43:14 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define RED "\x1B[91m"    // Lighter red
# define ORANGE "\x1B[33m" // Orange/yellow
# define GREEN "\x1B[32m"  // Green
# define BLUE "\x1B[94m"   // Light blue
# define BROWN "\x1B[31m"  // Approximation using red (no true brown in ANSI)
# define PURPLE "\x1B[35m" // Magenta (common substitute for purple)
# define RESET "\x1B[0m"
# define MAX_REASONABLE_SIZE 100

extern volatile sig_atomic_t	g_stop_flag; // declaration

typedef enum e_char_type
{
	CHAR_SPACE,
	CHAR_TEXT,
	CHAR_OPERATOR,
	CHAR_PARENTHESIS,
	CHAR_SINGLE_QUOTE,
	CHAR_DOUBLE_QUOTE,
	CHAR_NULL
}					t_char_type;

// Used for tokens but also during the token creation to type each char.
typedef enum e_token_type
{
	TOKEN_ERROR,
	TOKEN_CMD,			// ex: cat, sleep, ls ...
	TOKEN_ARGUMENT,		// ex: -a, -l,
						// file.txt ... (a file without a redirection)
	TOKEN_FILE,			// ex: > file.txt, file.txt ... (file with redirection)
	TOKEN_PIPE,			// |
	TOKEN_REDIRECT_OUT,	// >
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
	TOKEN_DOUBLE_QUOTE,
	TOKEN_SINGLE_QUOTE,
	TOKEN_EOF
}					t_token_type;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,	// |
	NODE_AND,	// &&
	NODE_OR,	// ||
}					t_node_type;

typedef struct s_env
{
	char			*name;
	char			*value;

	struct s_env	*next;
}					t_env;

typedef struct s_env_utils
{
	t_env			*head;
	t_env			*tail;
	t_env			*node;
	char			**tmp;
	int				i;
}					t_env_utils;

typedef struct s_expand
{
	char			saved;
	char			*expanded;
	size_t			var_len;
}					t_expand;

typedef struct s_redir
{
	t_token_type	type;
	char			*filename;
	int				heredoc_fd;
	struct s_redir	*next;
}					t_redir;

typedef struct s_node
{
	t_node_type		type;
	bool			builtin;
	bool			create_subshell;	// Trigger a subshell creation.
	int				in_subshell;		// increase each time we subshell.
	struct s_node	*left;
	struct s_node	*right;

	int				pipefd[2];
	pid_t			left_pid;
	pid_t			right_pid;

	t_env			*curr_env;
	char			**cmd_argv;
	t_redir			*redirection;
	char			*path;

	int				input_fd;
	int				output_fd;

	int				exit_status;
}					t_node;

typedef struct s_token
{
	t_token_type	type;
	char			*word;
	char			*slice;

	struct s_token	*prev_token;
	struct s_token	*next_token;
}					t_token;

typedef struct s_tokenizer
{
	char			*input;
	int				pos;
	int				prev_pos;
	int				length;
	int				depth;
	bool			in_word;

	char			next_char;

	t_token_type	curr_tok_type;
	t_token_type	prev_type;

	t_char_type		curr_char_type;
	t_char_type		prev_char_type;

	bool			double_quote;
	bool			single_quote;

	t_token			*token_list;
	t_token			*last_token;

	t_token			*token_array;

	int				token_list_size;	// Keep track of the number of tokens
}					t_tokenizer;

typedef struct s_main_data
{
	t_node			*node;
	t_list			*malloc_tok;
	t_list			*malloc_tree;

	char			*input;
	char			**argv;
	int				argc;

	int				error;
	char			*str_error;

	bool			in_child;

	void			*root;	// Pointer to root,
	// has to be casted at the beginning.
	// Copy of the root env, to be passed to execve,
	// it contains the local var of this command.
	t_env			*curr_env;

	t_tokenizer		*tok;
}					t_main_data;

typedef struct s_history
{
	char			**commands;
	int				capacity;
	int				count;
	int				current_pos;
}					t_history;

typedef struct s_root
{
	t_main_data		*data;

	int				last_exit_status;

	t_env			*env;

	t_list			*malloc_root;
	t_list			*list_of_list;
	bool			prompt_printed;
}					t_root;

typedef struct s_cmd_builder
{
	t_main_data		*data;
	t_token			*tok_array;
	t_node			*node;
	int				size;
}					t_cmd_builder;

/* -------------------- Lexer / Tokenizer -------------------- */
t_token_type		get_tok_type(char c, char next);
t_char_type			get_char_type(char c);
char				check_next_char(char *str, int pos);
int					tokenizer(t_main_data *data);
int					handle_quotes(t_tokenizer *tok, t_main_data *data, char c);
char				*clean_string(char *input);
int					create_token(t_main_data *data, int start, int end,
						t_token_type type);
t_token				*add_to_list(t_main_data *data, t_token *prev);
int					list_to_array(t_main_data *data, t_token *token_list,
						int size);

/* -------------------- Token helpers / expansion -------------------- */
t_token_type		get_word_type(t_token *tok);
int					expand_var(t_main_data *data, t_token *tok,
						t_token *removed, int *removed_dollar);
int					merge_with_prev_if_adjacent(t_main_data *data, t_token *tok,
						int start, int just_removed_dollar);
void				remove_token(t_token **head, t_token *t);
char				**split_env_var(t_root *root, char *env);
char				*get_expanded_var(t_main_data *data, const char *var_name);

/* -------------------- Parser / AST construction -------------------- */
t_node_type			map_token_to_node(t_token_type t);
int					is_redir(t_token_type type);
int					is_and_or(t_token_type t);
int					is_word_token(t_token_type t);
int					is_op_or_redir(t_token_type t);
int					is_operator(t_token_type t);
int					is_command_start(t_token_type t);
int					is_command_end(t_token_type t);
int					handle_parenthesis(t_main_data *data,
						t_token_type *tok_type);
int					handle_operator(t_main_data *data, t_token_type *tok_type);
int					check_paren_error(t_token *tok_list, int size);
int					wrapped_in_paren(t_token *tok_list, int size);
int					get_cmd_argc(t_token *tok_list, int size);
t_redir				*add_redirection(t_main_data *data, t_node *node,
						t_token_type type);
t_node				*build_tree(t_main_data *data, t_token *tok_list, int size,
						int depth);
t_node				*create_node(t_main_data *data, t_token *tok_array,
						t_node_type type, int size);

/* -------------------- Tree traversal
	/ Execution control -------------------- */
int					parser(t_main_data *data);
int					traverse_tree(t_node *node, t_main_data *data);
int					execution(t_node *node, t_main_data *data);
int					exec_handler(t_main_data *data, t_node *node);
int					exec_cmd(t_node *node, t_main_data *data);
int					and_and(t_node *node, t_main_data *data);
int					or_or(t_node *node, t_main_data *data);
int					pipes(t_node *node, t_main_data *data);
int					set_io_fds(t_node *node, t_main_data *data);
int					redirections(t_node *node, t_main_data *data);
int					open_file(char *filename, int action);
int					get_bin_path(t_node *node, t_main_data *data);
void				print_exec_error(int error, t_node *node);
int					heredoc(t_redir *redir, t_main_data *data);

/* -------------------- Builtins / Command helpers -------------------- */
int					exec_builtins(t_node *node, t_main_data *data);
int					is_builtin(char *cmd);
int					if_builtin(t_node *node, t_main_data *data);
int					exec_builtin_in_parent(t_node *node, t_main_data *data);
int					exit_builtin(t_node *node, t_main_data *data);
int					cd(t_root *root, t_node *node);
int					pwd(void);
int					echo(char **argv);
int					matrix(char **arg);

/* -------------------- Environment / vars / export
	/ unset -------------------- */
t_env				*copy_env(t_list **malloc_list, t_env *env);
t_env				*set_env_var_list(t_root *root, char **env);
char				*get_env_var(t_env *env, char *target);
char				*find_bin(t_env *env, char *bin);
t_env				*find_tenv_var(t_env *env, char *name);
int					export(t_root *root, char **var);
int					env(t_env *root_env);
int					unset(t_root *root, t_env **env, char **argv);
int					export_env(t_env *root_env);
char				**t_env_to_char_arr(t_root *root, t_env *list);
int					set_last_exit_status_var(t_root *root, int status);
char				*my_getcwd(t_root *root);
t_token_type		no_env_var(t_main_data *data);

/* -------------------- Signals / cleanup / utilities -------------------- */
void				handler(int sig);
void				handle_signals(void);
void				cleanup(t_main_data *data, t_root *root);
void				syntax_error(char *message);
int					initialize_shell(t_root *root, char *prompt,
						size_t prompt_size);
void				reset_tokenizer_for_line(t_tokenizer *tok, char *line);
int					init(t_root *root);
void				create_prompt(char *prompt, size_t size, t_root *root);
void				cleanup_after_command(t_main_data *data, char **line);
void				handle_eof(char **line);
void				handle_empty_input(char **line);
void				cleanup(t_main_data *data, t_root *root);
void				handle_signals_rl(void);
void				handle_signals(void);
void				handle_signals_heredoc(void);

int					process_heredocs(t_main_data *data);
int					process_heredocs_in_node(t_node *node, t_main_data *data);

#endif