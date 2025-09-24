#include "../minishell.h"

int del_one(t_env **head, const char *name)
{
    t_env   *prev;
    t_env   *curr;
    int  length;

    if (!head || !*head || !name)
        return (1);
    prev = NULL;
    curr = *head;
    length = ft_strlen(name);
    while (curr)
    {
        if (ft_strlen(curr->name) == length && ft_strncmp(curr->name, name, length) == 0)
        {
            if (prev)
                prev->next = curr->next;
            else
                *head = curr->next;
            free(curr);
            return (0);
        }
        prev = curr;
        curr = curr->next;
    }
    return (1);
}

int unset(t_env **env, char **argv)
{
    int i;
    int error;

    if (!env || !*env || !argv)
        return (1);
    error = 0;
    i = 0;
    while (argv[i])
    {
        if (del_one(env, argv[i]))
            error++;
        i++;
    }
    return (0);
}