#include "../minishell.h"

// Print execve error according
void print_exec_error(int error, t_node *node)
{
    const char *cmd;
	
	cmd = "unknown";

    if (node && node->cmd_argv && node->cmd_argv[0])
        cmd = node->cmd_argv[0];

    if (error == 126)
    {
        if (errno == EISDIR)
            fdprintf(2, "minishell: %s: Is a directory\n", cmd);
        else
            fdprintf(2, "minishell: %s: Permission denied\n", cmd);
    }
    else if (error == 127)
        fdprintf(2, "minishell: %s: command not found\n", cmd);
    else
    {
        if (errno == ENOEXEC)
            fdprintf(2, "minishell: %s: Exec format error\n", cmd);
        else if (errno == EISDIR)
            fdprintf(2, "minishell: %s: Is a directory\n", cmd);
        //else
            //fdprintf(2, "minishell: %s: %s\n", cmd, strerror(errno));
    }
}

void syntax_error(char *message)
{
	if (!message)
		return ;
	fdprintf(2, "minishell: syntax error near unexpected token `%s'\n", message);
}
