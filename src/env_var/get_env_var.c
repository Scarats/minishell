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

// Split at the first =
char **split_env_var(t_root *root, char *env)
{
	char **env_var;
	int i;
	int y;

	if (!env || !root)
		return (NULL);
	env_var = my_malloc(&root->list_of_list, &root->malloc_root, sizeof(char *) * 3); // for the end of env_var[3], env_var[0][i] and env_var[1][y]
	i = 0;
	y = -1;
	while (env[i] && env[i] != '=')	
		i++;
	env_var[0] = my_malloc(&root->list_of_list, &root->malloc_root, sizeof(char) * (i + 1));
	while (++y < i)
		env_var[0][y] = env[y];
	env_var[1] = my_malloc(&root->list_of_list, &root->malloc_root, sizeof(char) * ((ft_strlen(env) - i) + 1));
	y = 0;
	i++;
	while (env[i])
		env_var[1][y++] = env[i++];
	return (env_var);
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
        tmp = split_env_var(root, env[i]);
		if (!tmp || tmp[0] == NULL || tmp[1] == NULL)
			return (free_2d_array((void **)tmp), NULL);
        node = my_malloc(&root->list_of_list, &root->malloc_root, sizeof(t_env));
        node->name = tmp[0];
        node->value = tmp[1];
		node->next = NULL;
        if (!head)
            head = node;
        else
            tail->next = node;
        tail = node;
    }
    return (head);
}
