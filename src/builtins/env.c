#include "../minishell.h"

// Print env variables.
int env(t_env *root_env)
{
    t_env *ptr;

    if (!root_env)
        return (1);
    ptr = root_env;
    while (ptr)
    {
        printf("%s=%s\n", ptr->name, ptr->value);
        ptr = ptr->next;
    }
    return (0);
}
