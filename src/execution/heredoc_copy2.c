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

/* static void heredoc_child_signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        _exit(130);
    }
    if (sig == SIGQUIT)
        return;
} */

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

static int read_heredoc_input(int fd, char *delimiter, t_main_data *data)
{
    char *line;
    t_root *root;

    root = data->root;    
    while (1)
    {
        // Check for SIGINT interruption
        if (g_stop_flag)
        {
            root->last_exit_status = 130;
            return (-1);  // Abort heredoc
        }
        line = readline("> ");
        if (!line)  // EOF (Ctrl+D)
            break;
        if (is_delimiter_match(line, delimiter))
        {
            free(line);
            break;
        }
        ft_putendl_fd(line, fd);  // Write line + newline to temp file
        free(line);
    }
    return (0);
}

/* static int read_heredoc_input(int fd, char *delimiter)
{
    char *line;
    
    while (1)
    {
        line = readline("> ");
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
} */

/* static void setup_child_signals(void)
{
    struct sigaction sa_new;
    
    sa_new.sa_handler = heredoc_child_signal_handler;
    sigemptyset(&sa_new.sa_mask);
    sa_new.sa_flags = 0;
    sigaction(SIGINT, &sa_new, NULL);
    sigaction(SIGQUIT, &sa_new, NULL);
} */

/* static void attach_tty_for_readline(void)
{
    int tty = open("/dev/tty", O_RDWR);
    if (tty >= 0)
    {
        dup2(tty, STDIN_FILENO);
        dup2(tty, STDOUT_FILENO);
        close(tty);
    }
} */

/* static void	set_parent_heredoc_signals(struct sigaction *old_int,
        struct sigaction *old_quit)
{
    struct sigaction	ignore;

    ignore.sa_handler = SIG_IGN;
    sigemptyset(&ignore.sa_mask);
    ignore.sa_flags = 0;
    sigaction(SIGINT, &ignore, old_int);
    sigaction(SIGQUIT, &ignore, old_quit);
}

static void	restore_parent_signals(struct sigaction *old_int,
        struct sigaction *old_quit)
{
    sigaction(SIGINT, old_int, NULL);
    sigaction(SIGQUIT, old_quit, NULL);
} */

/* static int handle_child_process(int fd, t_redir *redir, t_main_data *data)
{
    setup_child_signals();
    attach_tty_for_readline();
    read_heredoc_input(fd, redir->filename);
    close(fd);
    my_multi_free(&data->root->list_of_list);
    exit(0);
} */

/* static int handle_wait_status(int status, t_main_data *data, char *filename)
{
    if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
    {
        stop_flag = 1;
        data->root->last_exit_status = 130;
        unlink(filename);
        return (-1);
    }
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        stop_flag = 1;
        data->root->last_exit_status = 130;
        unlink(filename);
        return (-1);
    }
    return (0);
} */

/* static int handle_wait_status(int status, t_main_data *data, char *filename)
{
    if (WIFEXITED(status))
    {
        int exit_code = WEXITSTATUS(status);
        if (exit_code == 130)
        {
            data->root->last_exit_status = 130;
            unlink(filename);
            return (-1);
        }
    }
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        data->root->last_exit_status = 130;
        unlink(filename);
        return (-1);
    }
    return (0);
} */

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

/* static int cleanup_and_return_error(int fd, char *filename)
{
    if (fd != -1)
        close(fd);
    if (filename)
    {
        unlink(filename);
        free(filename);
    }
    return (-1);
} */

/* static int handle_fork_and_wait(int fd, t_redir *redir, t_main_data *data, char *filename)
{
    int					pid;
    int					status;
    struct sigaction	old_int;
    struct sigaction	old_quit;

    set_parent_heredoc_signals(&old_int, &old_quit);
    pid = fork();
    if (pid == -1)
    {
        restore_parent_signals(&old_int, &old_quit);
        return (cleanup_and_return_error(fd, filename));
    }
    if (pid == 0)
        return (handle_child_process(fd, redir, data));
    close(fd);
    if (waitpid(pid, &status, 0) == -1)
    {
        restore_parent_signals(&old_int, &old_quit);
        return (cleanup_and_return_error(-1, filename));
    }
    restore_parent_signals(&old_int, &old_quit);
    if (handle_wait_status(status, data, filename) == -1)
        return (cleanup_and_return_error(-1, filename));
    return (0);
} */

/* static int handle_fork_and_wait(int fd, t_redir *redir, t_main_data *data, char *filename)
{
    int                 pid;
    int                 status;
    struct sigaction    ignore;
    struct sigaction    old_int;
    struct sigaction    old_quit;

    sigemptyset(&ignore.sa_mask);        // Make readline prompt/input use the terminal, not the pipe
    ignore.sa_flags = 0;
    ignore.sa_handler = SIG_IGN;
    sigaction(SIGINT, &ignore, &old_int);
    sigaction(SIGQUIT, &ignore, &old_quit);
    pid = fork();
    if (pid == -1)
    {
        sigaction(SIGINT, &old_int, NULL);
        sigaction(SIGQUIT, &old_quit, NULL);
        return (cleanup_and_return_error(fd, filename));
    }
    if (pid == 0)
        return (handle_child_process(fd, redir, data));
    close(fd);
    if (waitpid(pid, &status, 0) == -1)
    {
        sigaction(SIGINT, &old_int, NULL);
        sigaction(SIGQUIT, &old_quit, NULL);
        return (cleanup_and_return_error(-1, filename));
    }
    sigaction(SIGINT, &old_int, NULL);
    sigaction(SIGQUIT, &old_quit, NULL);
    if (handle_wait_status(status, data, filename) == -1)
        return (cleanup_and_return_error(-1, filename));
    return (0);
} */

static int cleanup_and_return_error(int fd, char *filename)
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

static int open_and_assign_filename(char *filename, t_redir *redir, t_main_data *data)
{
    int read_fd;

    (void)redir;
    (void)data;
    read_fd = open(filename, O_RDONLY);
    if (read_fd == -1)
    {
        unlink(filename);
        free(filename);
        return (-1);
    }
    unlink(filename);  // Remove temp file; fd keeps data alive
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
    if (read_heredoc_input(fd, redir->filename, data) == -1)
    {
        cleanup_and_return_error(fd, filename);
        return (-1);
    }
    close(fd);
    return (open_and_assign_filename(filename, redir, data));
}
