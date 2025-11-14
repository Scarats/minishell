#include "../minishell.h"

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