#include "../minishell.h"

// Print execve error according
void print_exec_error(int error, t_node *node)
{
	if (error == 126)
        fprintf(stderr, "minishell: %s: Permission denied\n", argv[0]);
    case ENOENT:
        fprintf(stderr, "minishell: %s: No such file or directory\n", argv[0]);
        exit(127); // same as Bash
    case ENOEXEC:
        fprintf(stderr, "minishell: %s: Exec format error\n", argv[0]);
        exit(126);
    case EISDIR:
        fprintf(stderr, "minishell: %s: Is a directory\n", argv[0]);
        exit(126);
    default:
        fprintf(stderr, "minishell: %s: %s\n", argv[0], strerror(errno));
        exit(126);
}

void syntax_error(char *message)
{
	if (!message)
		return ;
	fdprintf(2, "minishell: syntax error near unexpected token `%s'\n", message);
}
