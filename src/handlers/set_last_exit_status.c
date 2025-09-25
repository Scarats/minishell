#include "../minishell.h"

int	set_last_exit_status_var(t_root *root)
{
    char	*itoa_val;
    int		length;
    char	*exit_var;

    itoa_val = ft_itoa(root->last_exit_status);
    if (!itoa_val)
        return (-1);
    length = ft_strlen(itoa_val) + 3; /* 2 for "?=", 1 for '\0' */
    exit_var = my_malloc(&root->list_of_list, &root->malloc_root, length);
    exit_var[0] = '?';
    exit_var[1] = '=';
    ft_strlcpy(exit_var + 2, itoa_val, length - 2);
    free(itoa_val);
    export(root, exit_var);
    return (0);
}
