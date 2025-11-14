#include "../minishell.h"

static void setup_child_signals(void)
{
    struct sigaction sa_new;
    
    sa_new.sa_handler = heredoc_child_signal_handler;
    sigemptyset(&sa_new.sa_mask);
    sa_new.sa_flags = 0;
    sigaction(SIGINT, &sa_new, NULL);
    sigaction(SIGQUIT, &sa_new, NULL);
}

static int handle_child_process(int fd, t_redir *redir, t_main_data *data)
{
    t_root *root;

    root = data->root; 
    setup_child_signals();
    attach_tty_for_readline();
    read_heredoc_input(fd, redir->filename);
    close(fd);
    my_multi_free(&root->list_of_list);
    exit(0);
}

static int handle_wait_status(int status, t_main_data *data, char *filename)
{
    t_root *root;

    root = data->root; 
    if (WIFEXITED(status))
    {
        int exit_code = WEXITSTATUS(status);
        if (exit_code == 130)
        {
            root->last_exit_status = 130;
            g_stop_flag = 1;
            unlink(filename);
            return (-1);
        }
    }
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        root->last_exit_status = 130;
        g_stop_flag = 1;
        unlink(filename);
        return (-1);
    }
    return (0);
}

static int create_heredoc_file(t_main_data *data, char **filename)
{
    int fd;
    
    *filename = generate_heredoc_filename(data);
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
static int wait_heredoc_child(pid_t pid, t_root *root)
{
    int status;
    struct sigaction oldint, oldquit, ign;

    ign.sa_handler = SIG_IGN;
    sigemptyset(&ign.sa_mask);
    ign.sa_flags = 0;
    sigaction(SIGINT, &ign, &oldint);
    sigaction(SIGQUIT, &ign, &oldquit);
    if (waitpid(pid, &status, 0) == -1)
        status = -1;
    sigaction(SIGINT, &oldint, NULL);
    sigaction(SIGQUIT, &oldquit, NULL);
    if (status != -1 && WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        root->last_exit_status = 130;
        g_stop_flag = 1;
        return (-1);
    }
    return (0);
}

static int handle_fork_and_wait(int fd, t_redir *redir, t_main_data *data, char *filename)
{
    int pid, status;
    t_root *root;
    
    root = data->root; 
    pid = fork();
    if (pid == -1)
        return (cleanup_and_return_error(fd, filename));
    if (pid == 0)
        return (handle_child_process(fd, redir, data));
    close(fd);
    if (waitpid(pid, &status, 0) == -1)
        return (cleanup_and_return_error(-1, filename));
    if (handle_wait_status(status, data, filename) == -1)
    {
        handle_signals();
        return (cleanup_and_return_error(-1, filename));
    }
    else
    {
        if(wait_heredoc_child(pid, root))
            return(-1);
    }
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
    
    fd = create_heredoc_file(data, &filename);
    if (fd == -1)
        return (-1);
    if (handle_fork_and_wait(fd, redir, data, filename) == -1)
        return (-1);
    return (open_and_assign_filename(filename, redir, data));
}