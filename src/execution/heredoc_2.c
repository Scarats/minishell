#include "../minishell.h"

void setup_child_signals(void)
{
    struct sigaction sa_new;
    
    sa_new.sa_handler = heredoc_child_signal_handler;
    sigemptyset(&sa_new.sa_mask);
    sa_new.sa_flags = 0;
    sigaction(SIGINT, &sa_new, NULL);
    sigaction(SIGQUIT, &sa_new, NULL);
}
int handle_child_process(int fd, t_redir *redir, t_main_data *data)
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

int create_heredoc_file(t_main_data *data, char **filename)
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