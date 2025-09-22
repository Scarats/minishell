#include "../minishell.h"

t_env *copy_env(t_list *malloc_list, t_env *env)
{
    t_env *new_head;
    t_env *new_tail;
    t_env *node;

    if (!env || !malloc_list)
        return (NULL);
    new_head = NULL;
    new_tail = NULL;
    while (env != NULL)
    {
        node = my_malloc(&malloc_list, sizeof(t_env));
        if (!node)
            return (NULL);
        node->name = my_strdup(malloc_list, env->name);
        node->value = my_strdup(malloc_list, env->value);
        if (!node->name || !node->value)
            return (NULL);
        if (!new_head)
            new_head = node;
        else
            new_tail->next = node;
        new_tail = node;
        env = env->next;
    }
    return (new_head);
}
