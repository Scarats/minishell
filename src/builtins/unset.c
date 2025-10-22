#include "../minishell.h"

void	ici_ca_bz(t_env *prev, t_root *root, t_env *curr, t_env **head)
{
	if (prev)
	{
		my_free_one(&root->malloc_root, (void *)curr->name);
		my_free_one(&root->malloc_root, (void *)curr->value);
		prev->next = curr->next;
	}
	else
	{
		my_free_one(&root->malloc_root, (void *)curr->name);
		my_free_one(&root->malloc_root, (void *)curr->value);
		*head = curr->next;
	}
}

int	del_one(t_root *root, t_env **head, const char *name)
{
	t_env	*prev;
	t_env	*curr;
	//int		length;

	if (!head || !*head || !name)
		return (1);
	prev = NULL;
	curr = *head;
	//length = ft_strlen(name);
	while (curr)
	{
		if (curr->name && !ft_strcmp(curr->name, name))
			return (ici_ca_bz(prev, root, curr, head), 0);
		prev = curr;
		curr = curr->next;
	}
	return (1);
}

int	unset(t_root *root, t_env **env, char **argv)
{
	int i;
	int error;

	if (!env || !*env || !argv)
		return (1);
	error = 0;
	i = 0;
	while (argv[i])
	{
		if (del_one(root, env, argv[i]))
			error++;
		i++;
	}
	return (error);
}
