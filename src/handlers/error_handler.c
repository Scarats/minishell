#include "../minishell.h"

void syntax_error(char *message)
{
	if (!message)
		return ;
	fdprintf(2, "minishell: syntax error near unexpected token `%s'\n", message);
}
