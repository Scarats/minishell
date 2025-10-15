#include "minishell.h"

volatile sig_atomic_t	stop_flag = 0;

int	init(t_main_data *data)
{
	data->tok = malloc(sizeof(t_tokenizer));
	if (!data->tok)
		return (1);
	ft_memset(data->tok, 0, sizeof(t_tokenizer));
	data->tok->prev_char_type = CHAR_SPACE;
	data->tok->curr_char_type = CHAR_NULL;
	data->tok->double_quote = false;
	data->tok->single_quote = false;
	data->node = NULL;
	data->malloc_tok = NULL;
	data->malloc_tree = NULL;
	data->error = 0;
	data->in_child = false;
	data->last_exit_status = 0;
	return (0);
}

static void	reset_tokenizer_for_line(t_tokenizer *tok, char *line)
{
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
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	handler(int sig)
{
	(void)sig;
	stop_flag = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	cleanup(t_main_data *data)
{
	my_free(&data->malloc_tok);
	my_free(&data->malloc_tree);
	if (data->tok)
		free(data->tok);
	write_history(".minishell_history");
}

static int	initialize_shell(t_main_data *data, char *prompt,
		size_t prompt_size)
{
	create_prompt(prompt, prompt_size);
	handle_signals();
	if (init(data) != 0)
	{
		fprintf(stderr, "Initialization failed\n");
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
	reset_tokenizer_for_line(data->tok, line);
	if (parser(data) == 0)
		data->last_exit_status = traverse_tree(data->node, data);
	else
	{
		fprintf(stderr, "minishell: syntax error\n");
		data->last_exit_status = 2;
	}
	return (data->last_exit_status);
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
	add_history(line);
	if (!ft_strncmp(line, "exit", 5))
	{
		free(line);
		return (false);
	}
	process_command(data, line);
	cleanup_after_command(data, &line);
	return (true);
}

int	main(void)
{
	t_main_data	data;
	char		prompt[256];

	if (initialize_shell(&data, prompt, sizeof(prompt)) != 0)
		return (1);
	while (execute_command_loop(&data, prompt))
		;
	cleanup(&data);
	return (data.last_exit_status);
}
