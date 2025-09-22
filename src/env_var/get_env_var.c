#include "../minishell.h"

t_env *get_env_var(t_list *malloc_list, char **env)
{
    t_env	*head;
    t_env	*tail;
    t_env	*node;
    char	**tmp;
    int		i;

    if (!env)
        return (NULL);
    head = NULL;
    tail = NULL;
    i = -1;
    while (env[++i])
    {
        tmp = ft_split(env[i], '=');
        if (!tmp || tmp[0] == NULL || tmp[1] == NULL)

        node = my_malloc(&malloc_list, sizeof(t_env));
        node->name = tmp[0];
        node->value = tmp[1];
        free(tmp);
        if (!head)
            head = node;
        else
            tail->next = node;
        tail = node;
    }
    return (head);
}
