#include "../minishell.h"

// Find the var's value in the shell's environment.
char *get_env_var(t_env *env, char *target)
{
	t_env *ptr;

	if (!env || !target)
		return (NULL);
	ptr = env;
	while (ptr)
	{
		if (!ft_strcmp(target, ptr->name))
			return (ptr->value);
		ptr = ptr->next;
	}
	return (NULL);
}

// Create a t_env struct from a char **, spliting NAME=value.
t_env *set_env_var_list(t_root *root, char **env)
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
			return (free_2d_array((void **)tmp), NULL);
        node = my_malloc(&root->list_of_list, &root->malloc_root, sizeof(t_env));
        node->name = my_strdup(&root->malloc_root, tmp[0]);
        node->value = my_strdup(&root->malloc_root, tmp[1]);
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
