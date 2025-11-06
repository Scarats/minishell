#include "../minishell.h"

static char *generate_heredoc_filename(void)
{
    static int counter = 0;
    char *filename;
    char *tmp;
    
    tmp = ft_itoa(counter++);
    if (!tmp)
        return (NULL);
    filename = ft_strjoin("/tmp/minishell_heredoc_", tmp);
    free(tmp);
    return (filename);
}

void	handler_rl(int sig)
{
    if (sig == SIGINT)
    {
        g_stop_flag = 1;
        rl_on_new_line();
        rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
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

void	handle_signals_rl(void)
{
    struct sigaction	sa_int;
    struct sigaction	sa_quit;

    sa_int.sa_handler = handler_rl;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);
    
    sa_quit.sa_handler = handler_rl;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void	handle_signals(void)
{
    struct sigaction	sa_int;
    struct sigaction	sa_quit;

    sa_int.sa_handler = handler;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);
    
    sa_quit.sa_handler = handler;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void	handler_heredoc(int sig)
{
	if (sig == SIGINT)
    {
        g_stop_flag = 1;
        //srl_on_new_line();
		//
		rl_on_new_line();
        rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
		close(STDIN_FILENO);
		//printf("Closing\n");
		//rl_done = 1;
		//close(STDIN_FILENO);
		//rl_replace_line("", 0);
        //rl_redisplay();
    }
}

void	handle_signals_heredoc(void)
{
    struct sigaction	sa_int;
    struct sigaction	sa_quit;

    sa_int.sa_handler = handler_heredoc;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);
    
    sa_quit.sa_handler = handler;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void	handler(int sig)
{
    if (sig == SIGINT)
    {
        g_stop_flag = 1;
		//write(STDOUT_FILENO, "\n", 1);
    }
    else if (sig == SIGQUIT)
    {
        write(STDOUT_FILENO, "minishell: quit (core dumped)\n", 31);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}



static int is_delimiter_match(char *line, char *delimiter)
{
    size_t delim_len;
    
    if (!delimiter)
        return (0);
    delim_len = ft_strlen(delimiter);
    if (ft_strncmp(line, delimiter, delim_len) == 0 && line[delim_len] == '\0')
        return (1);
    return (0);
}

static int read_heredoc_input(int fd, char *delimiter)
{
    char *line;
    
    while (1)
    {
        line = readline("> ");
		if (g_stop_flag)
			break;
        if (!line)
            break;
        if (is_delimiter_match(line, delimiter))
        {
            free(line);
            break;
        }
        ft_putendl_fd(line, fd);
        free(line);
    }
    return (0);
}

void setup_child_signals(void)
{
    /*struct sigaction sa_new;
    
    sa_new.sa_handler = heredoc_child_signal_handler;
    sigemptyset(&sa_new.sa_mask);
    sa_new.sa_flags = 0;
    sigaction(SIGINT, &sa_new, NULL);
    sigaction(SIGQUIT, &sa_new, NULL);*/
}

static int handle_child_process(int fd, t_redir *redir, t_main_data *data)
{
	(void)data;
    //setup_child_signals();
    read_heredoc_input(fd, redir->filename);
    close(fd);
    //my_multi_free(&data->root->list_of_list);
    //exit(0);
	return (0);
}

int handle_wait_status(int status, t_main_data *data, char *filename)
{
    t_root *root;

    root = data->root;
    if (WIFEXITED(status))
    {
        int exit_code = WEXITSTATUS(status);
        if (exit_code == 130)
        {
            root->last_exit_status = 130;
            unlink(filename);
            return (-1);
        }
    }
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        root->last_exit_status = 130;
        unlink(filename);
        return (-1);
    }
    return (0);
}

static int create_heredoc_file(char **filename)
{
    int fd;
    
    *filename = generate_heredoc_filename();
    if (!*filename)
        return (-1);
    fd = open(*filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1)
    {
        free(*filename);
        return (-1);
    }
    return (fd);
}

int cleanup_and_return_error(int fd, char *filename)
{
    if (fd != -1)
        close(fd);
    if (filename)
    {
        unlink(filename);
        free(filename);
    }
    return (-1);
}

static int handle_fork_and_wait(int fd, t_redir *redir, t_main_data *data, char *filename)
{
	(void)filename;
	(void)data;
   // int pid, status;
    
    //pid = fork();
    //if (pid == -1)
       // return (cleanup_and_return_error(fd, filename));
    //if (pid == 0)
    handle_child_process(fd, redir, data);
    close(fd);
	//handle_signals_heredoc();
    //if (waitpid(pid, &status, 0) == -1)
        //return (cleanup_and_return_error(-1, filename));
	
    /*if (handle_wait_status(status, data, filename) == -1)
    {
        return (cleanup_and_return_error(-1, filename));
    }*/
    return (0);
}

static int open_and_assign_filename(char *filename, t_redir *redir, t_main_data *data)
{
    int read_fd;
    t_root *root;

    root = data->root;    
    read_fd = open(filename, O_RDONLY);
    if (read_fd == -1)
    {
        unlink(filename);
        free(filename);
        return (-1);
    }
    redir->filename = my_strdup(&root->list_of_list, filename);
    free(filename);
    return (read_fd);
}

int heredoc(t_redir *redir, t_main_data *data)
{
    int fd;
    char *filename;
    
    fd = create_heredoc_file(&filename);
    if (fd == -1)
        return (-1);

    handle_signals_heredoc();

	
    if (handle_fork_and_wait(fd, redir, data, filename) == -1)
        return (-1);
    if (g_stop_flag)
	{
		// SHould clean up
		//printf("Closing\n");
		exit(130);
	}
    return (open_and_assign_filename(filename, redir, data));
}
