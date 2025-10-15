#include "../minishell.h"

int	set_last_exit_status_var(t_root *root, int status)
{
    char	*num;
    char	*kv;
    char	*args[2];

    num = ft_itoa(status);
    if (!num)
        return (fdprintf(2, "minishell: allocation error\n"), 1);
    kv = ft_strjoin("?=", num);
    if (!kv)
        return (fdprintf(2, "minishell: allocation error\n"), 1);
    args[0] = kv;
    args[1] = NULL; /* IMPORTANT: NULL-terminate the argv array */
    return (export(root, args));
}
