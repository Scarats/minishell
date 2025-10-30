#include "../minishell.h"

// Create a unique temp filename under /tmp
static char *generate_heredoc_filename(void)
{
    static int counter;
    char *num = ft_itoa(counter++);
    char *name;

    if (!num)
        return NULL;
    name = ft_strjoin("/tmp/minishell_heredoc_", num);
    free(num);
    return name;
}

static int is_delimiter_match(char *line, const char *delim)
{
    size_t dlen;

    if (!line || !delim)
        return 0;
    dlen = ft_strlen(delim);
    return (ft_strncmp(line, delim, dlen) == 0 && line[dlen] == '\0');
}

// Child signal handler: Ctrl-C aborts heredoc with code 130, Ctrl-\ ignored
static void heredoc_child_signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        _exit(130);
    }
    // SIGQUIT: do nothing
}

static void setup_child_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = heredoc_child_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

// Ensure readline uses the terminal even if stdin/stdout were redirected
static void attach_tty_for_readline(void)
{
    int tty = open("/dev/tty", O_RDWR);
    if (tty >= 0)
    {
        dup2(tty, STDIN_FILENO);
        dup2(tty, STDOUT_FILENO);
        close(tty);
    }
}

// Parent ignores SIGINT/SIGQUIT while waiting for child heredoc
static void set_parent_heredoc_signals(struct sigaction *old_int,
                                       struct sigaction *old_quit)
{
    struct sigaction ign;

    ign.sa_handler = SIG_IGN;
    sigemptyset(&ign.sa_mask);
    ign.sa_flags = 0;
    sigaction(SIGINT, &ign, old_int);
    sigaction(SIGQUIT, &ign, old_quit);
}

static void restore_parent_signals(struct sigaction *old_int,
                                   struct sigaction *old_quit)
{
    sigaction(SIGINT, old_int, NULL);
    sigaction(SIGQUIT, old_quit, NULL);
}

static int cleanup_and_return_error(int fd, char *filename)
{
    if (fd != -1)
        close(fd);
    if (filename)
    {
        unlink(filename);
        free(filename);
    }
    return -1;
}

// Read from user until delimiter line, write lines (with newline) into fd
static void read_heredoc_input(int fd, const char *delimiter)
{
    char *line;

    for (;;)
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
}

static int open_and_consume_tmp(char *filename)
{
    int read_fd = open(filename, O_RDONLY);

    if (read_fd == -1)
        return cleanup_and_return_error(-1, filename);
    unlink(filename); // remove path; fd keeps content alive
    free(filename);
    return read_fd;
}

// Public API used by redirections(): returns read-fd on success, else -1
int heredoc(t_redir *redir, t_main_data *data)
{
    int                 wstatus;
    int                 pid;
    int                 fd = -1;
    char                *filename = NULL;
    struct sigaction    old_int, old_quit;
    t_root *root;

    root = data->root;

    // Create temp file for child to write into
    filename = generate_heredoc_filename();
    if (!filename)
        return -1;
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1)
        return cleanup_and_return_error(-1, filename);

    set_parent_heredoc_signals(&old_int, &old_quit);
    pid = fork();
    if (pid == -1)
    {
        restore_parent_signals(&old_int, &old_quit);
        return cleanup_and_return_error(fd, filename);
    }
    if (pid == 0)
    {
        // Child: setup signals and readline, write to temp, then exit
        setup_child_signals();
        attach_tty_for_readline();
        read_heredoc_input(fd, redir->filename);
        close(fd);
        my_multi_free(&root->list_of_list);
        _exit(0);
    }
    // Parent
    close(fd);
    while (waitpid(pid, &wstatus, 0) == -1 && errno == EINTR)
        ;
    restore_parent_signals(&old_int, &old_quit);

    // Handle child termination status
    if (WIFEXITED(wstatus))
    {
        int code = WEXITSTATUS(wstatus);
        if (code == 130) // interrupted with Ctrl-C
        {
            root->last_exit_status = 130;
            root->heredoc_aborted = 1;   // <- mark abort of the whole line
            //errno = EINTR;               // <- let caller distinguish interrupt
            return cleanup_and_return_error(-1, filename);
        }
    }
    else if (WIFSIGNALED(wstatus) && WTERMSIG(wstatus) == SIGINT)
    {
        root->last_exit_status = 130;
        root->heredoc_aborted = 1;       // <- mark abort of the whole line
        //errno = EINTR;                   // <- let caller distinguish interrupt
        return cleanup_and_return_error(-1, filename);
    }

    // Success: reopen for reading, unlink path, return fd
    return open_and_consume_tmp(filename);
}
