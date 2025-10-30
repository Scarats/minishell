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
    
    while (1)
    {
        // Check for SIGINT interruption
        if (g_stop_flag)
        {
            data->root->last_exit_status = 130;
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
