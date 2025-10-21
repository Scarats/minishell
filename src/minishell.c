#include "minishell.h"

volatile sig_atomic_t	stop_flag = 0;

int	init(t_main_data *data)
{
	data->tok = my_malloc(&data->root->list_of_list, &data->root->malloc_root,
			sizeof(t_tokenizer));
	if (!data->tok)
		return (1);
	data->tok->prev_char_type = CHAR_SPACE;
	data->tok->curr_char_type = CHAR_NULL;
	data->tok->double_quote = false;
	data->tok->single_quote = false;
	data->node = NULL;
	data->malloc_tok = NULL;
	data->malloc_tree = NULL;
	data->error = 0;
	data->in_child = false;
	data->root->last_exit_status = 0;
	return (0);
}

static void	reset_tokenizer_for_line(t_tokenizer *tok, char *line)
{
	printf(RED "RESET TOK\n" RESET);
	printf(RED "RESET TOK\n" RESET);
	tok->input = line;
	tok->length = (int)ft_strlen(line);
	tok->pos = 0;
	tok->prev_pos = 0;
	tok->prev_char_type = CHAR_SPACE;
	tok->curr_char_type = CHAR_NULL;
	tok->double_quote = false;
	tok->single_quote = false;
	tok->token_list = NULL;
	tok->last_token = NULL;
	tok->token_list_size = 0;
}

static void	create_prompt(char *prompt, size_t size)
{
	char	hostname[64] = {0};
	char	username[64] = {0};

	printf(RED "CREATE PROMPT\n" RESET);
	printf(RED "CREATE PROMPT\n" RESET);
	gethostname(hostname, sizeof(hostname) - 1);
	getlogin_r(username, sizeof(username) - 1);
	ft_bzero(prompt, size);
	ft_strlcat(prompt, BLUE, size);
	ft_strlcat(prompt, username, size);
	ft_strlcat(prompt, RESET "@" GREEN, size);
	ft_strlcat(prompt, hostname, size);
	ft_strlcat(prompt, RESET ":" PURPLE "minishell" RESET "> ", size);
}

void	handle_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	
	sa_quit.sa_handler = handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		stop_flag = 1;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		write(STDOUT_FILENO, "minishell: quit (core dumped)\n", 31);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	cleanup(t_main_data *data, t_root *root)
{
	my_free(&data->malloc_tok);
	my_free(&data->malloc_tree);
	my_multi_free(&root->list_of_list);
	write_history(".minishell_history");
}

static int	initialize_shell(t_main_data *data, char *prompt,
		size_t prompt_size)
{
	printf(RED "INIT SHELL\n" RESET);
	printf(RED "INIT SHELL\n" RESET);
	create_prompt(prompt, prompt_size);
	handle_signals();
	if (init(data))
	{
		fdprintf(2, "Initialization failed\n");
		return (1);
	}
	read_history(".minishell_history");
	return (0);
}

static void	handle_empty_input(char **line)
{
	free(*line);
	*line = NULL;
}

static void	handle_eof(char **line)
{
	write(STDOUT_FILENO, "exit\n", 5);
	free(*line);
	*line = NULL;
}

static int	process_command(t_main_data *data, char *line)
{
	printf(RED "PROCESS CMD\n" RESET);
	printf(RED "PROCESS CMD\n" RESET);
	reset_tokenizer_for_line(data->tok, line);
	printf(GREEN "BEFORE PARSER\n" RESET);
	if (!parser(data))
		data->root->last_exit_status = traverse_tree(data->node, data);
	else
		return (1);
	return (data->root->last_exit_status);
}

static void	cleanup_after_command(t_main_data *data, char **line)
{
	my_free(&data->malloc_tok);
	my_free(&data->malloc_tree);
	data->node = NULL;
	stop_flag = 0;
	free(*line);
	*line = NULL;
}

static bool	execute_command_loop(t_main_data *data, char *prompt)
{
	char	*line;

	printf(RED "EXEC CMD\n" RESET);
	printf(RED "EXEC CMD\n" RESET);
	line = NULL;
	fflush(stdout);
	line = readline(prompt);
	if (!line)
	{
		handle_eof(&line);
		return (false);
	}
	if (line[0] == '\0')
	{
		handle_empty_input(&line);
		return (true);
	}
	if (!ft_strncmp(line, "exit", 5))
	{
		free(line);
		line = NULL;
		return (false);
	}
	add_history(line);
	process_command(data, line);
	cleanup_after_command(data, &line);
	return (true);
}

int	main(int ac, char **av, char **envp)
{
	t_main_data	data;
	t_root		root;
	char		prompt[1024];
	int			exit_status;

	ft_memset(&root, 0, sizeof(root));
	ft_memset(&data, 0, sizeof(data));
	(void)ac;
	(void)av;
	exit_status = 0;
	root.malloc_root = NULL;
	root.env = NULL;
	root.list_of_list = NULL;
	root.data = &data;
	data.root = &root;
	root.env = set_env_var_list(&root, envp);
	if (initialize_shell(&data, prompt, sizeof(prompt)))
		return (1);
	printf(RED "INIT DONE\n" RESET);
	while (execute_command_loop(&data, prompt))
		;
	exit_status = root.last_exit_status;
	cleanup(&data, &root);
	ft_lstclear(&root.list_of_list, NULL);
	my_free(&root.malloc_root);
	return (exit_status);
}
