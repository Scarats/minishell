#include "../minishell.h"

// Find the var's value in the shell's environment.
char	*get_env_var(t_env *env, char *target)
{
	if (!env || !target)
		return (NULL);
	while (env)
	{
		if (env->name && ft_strncmp(env->name, target, ft_strlen(env->name)
				+ 1) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static void	int_to_str(int num, char *buffer)
{
	char	temp_buffer[32];
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (num == 0)
	{
		buffer[0] = '0';
		buffer[1] = '\0';
		return ;
	}
	while (num > 0)
	{
		temp_buffer[i++] = (num % 10) + '0';
		num /= 10;
	}
	while (i > 0)
		buffer[j++] = temp_buffer[--i];
	buffer[j] = '\0';
}

char	*get_special_var(t_main_data *data, const char *var_name)
{
	static char	buffer[32];

	if (ft_strncmp(var_name, "?", 1) == 0)
	{
		int_to_str(data->root->last_exit_status, buffer);
		return (buffer);
	}
	return (NULL);
}

char	*get_expanded_var(t_main_data *data, const char *var_name)
{
	char	*special_var;

	special_var = get_special_var(data, var_name);
	if (special_var)
		return (special_var);
	return (get_env_var(data->root->env, (char *)var_name));
}

// Create a t_env struct from a char **, spliting NAME=value.
t_env	*set_env_var_list(t_root *root, char **env)
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
		node = my_malloc(&root->list_of_list, &root->malloc_root,
				sizeof(t_env));
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
