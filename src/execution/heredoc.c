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
    char *trimmed_line;
    char *trimmed_delim;
    int match = 0;
    
    if (!line || !delimiter)
        return (0);
    
    // Trim both line and delimiter for proper comparison
    trimmed_line = ft_strtrim(line, " \t\n");
    trimmed_delim = ft_strtrim(delimiter, " \t\n");
    
    if (trimmed_line && trimmed_delim)
        match = (ft_strcmp(trimmed_line, trimmed_delim) == 0);
    
    if (trimmed_line) free(trimmed_line);
    if (trimmed_delim) free(trimmed_delim);
    return (match);
}

static void setup_heredoc_signals(void)
{
    struct sigaction sa;
    
    sa.sa_handler = SIG_DFL;  // Default signal handling in child
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

static int read_heredoc_input(int fd, char *delimiter)
{
    char *line;
    
    setup_heredoc_signals();  // Set up proper signal handling for heredoc
    
    while (1)
    {
        line = readline("> ");
        if (!line)  // EOF (Ctrl+D)
            break;
        if (is_delimiter_match(line, delimiter))
        {
            printf("eof\n");
            free(line);
            break;
        }
        ft_putendl_fd(line, fd);
        free(line);
    }
    return (0);
}

static int handle_child_process(int fd, t_redir *redir, t_main_data *data)
{
    (void)data;
    read_heredoc_input(fd, redir->filename);
    close(fd);
    exit(0);  // Child must exit
}

static int handle_fork_and_wait(int fd, t_redir *redir, t_main_data *data, char *filename)
{
    pid_t pid;
    int status;
    t_root *root = data->root;
    
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (-1);
    }
    
    if (pid == 0)  // Child process
    {
        handle_child_process(fd, redir, data);
    }
    
    // Parent process
    close(fd);
    
    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return (-1);
    }
    
    // Handle child exit status
    if (WIFEXITED(status))
    {
        int exit_code = WEXITSTATUS(status);
        if (exit_code == 130)  // SIGINT
        {
            root->last_exit_status = 130;
            unlink(filename);
            return (-1);
        }
    }
    else if (WIFSIGNALED(status))
    {
        int sig = WTERMSIG(status);
        if (sig == SIGINT)
        {
            root->last_exit_status = 130;
            unlink(filename);
            return (-1);
        }
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
        *filename = NULL;
        return (-1);
    }
    return (fd);
}

static int open_and_assign_filename(char *filename, t_redir *redir, t_main_data *data)
{
    int read_fd;
    t_root *root = data->root;
    
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
    printf("heredoc exec\n");
    if (fd == -1)
        return (-1);
    
    if (handle_fork_and_wait(fd, redir, data, filename) == -1)
    {
        if (filename)
            free(filename);
        return (-1);
    }
    
    return (open_and_assign_filename(filename, redir, data));
}