#include "../minishell.h"

// Swap the values stored in node, not the node directly.
void swap_node(t_env *n1, t_env *n2)
{
	char *name;
	char *value;

	if (!n1 || !n2)
		return ;
	name = n1->name;
	value = n1->value;
	
	n1->name = n2->name;
	n1->value = n2->value;

	n2->name = name;
	n2->value = value;
}

int is_sorted(t_env *env)
{
	t_env *tmp;

	if (!env)
		return (1);
	tmp = env;
	while (tmp->next)
	{
		if (ft_strcmp(tmp->name, tmp->next->name) > 0)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

void sort_env(t_env *env)
{
	t_env *ptr;

	if (!env)
		return ;
	ptr = env;
	while (!is_sorted(env))
	{
		if (ptr == NULL)
			ptr = env;
		if (ptr->next && ft_strcmp(ptr->name, ptr->next->name) > 0)
			swap_node(ptr, ptr->next);
		ptr = ptr->next;
	}
}

int export_env(t_env *root_env)
{
    t_env *ptr;

    if (!root_env)
        return (1);
    ptr = root_env;
	sort_env(ptr);
    while (ptr->next)
    {
        printf("export %s=\"%s\"\n", ptr->name, ptr->value);
        ptr = ptr->next;
    }
    return (0);
}

// Print n1 variables.
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
