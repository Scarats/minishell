#include "../minishell.h"

// Find the var's value in the shell's environment.
char *get_env_var(t_env *env, char *target)
{
    if (!env || !target)
        return (NULL);
    while (env)
    {
        if (env->name && ft_strncmp(env->name, target, ft_strlen(env->name) + 1) == 0)
            return (env->value);
        env = env->next;
    }
    return (NULL);
}


// Create a t_env struct from a char **, spliting NAME=value.
t_env *set_env_var_list(t_list **malloc_list, char **env)
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
			return (NULL);
        node = my_malloc(malloc_list, sizeof(t_env));
        node->name = my_strdup(malloc_list, tmp[0]);
        node->value = my_strdup(malloc_list, tmp[1]);
		node->next = NULL;
		free_2d_array((void **)tmp);
        if (!head)
            head = node;
        else
            tail->next = node;
        tail = node;
    }
    return (head);
}
