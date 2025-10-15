#include "../minishell.h"

int check_syntax(char *name)
{
	int i;

	if (!name || !name[0])
		return (1);
	if (!(ft_isalpha(name[0]) || name[0] == '_'))
		return (1);
	i = 1;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (1);
		i++;
	}
	return (0);
}

void clean_prev_env_var(t_root *root, char *src)
{
	char *name_arr[2];

	name_arr[0] = src;
	name_arr[1] = NULL;
	unset(&root->env, name_arr);
}

void add_var_to_list(t_root *root, t_env **head, t_env *new_var, char **args)
{
	t_env *ptr;

	if (!head || !new_var)
		return;
	if (!*head)
		*head = new_var;
	else
	{
		ptr = *head;
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = new_var;
	}
	my_array_addtolist(&root->malloc_root, (void **)args);
	new_var->exported = true;
	new_var->name = args[0];
	new_var->value = args[1];
	new_var->next = NULL;
}

int set_new_var(t_root *root, char *var)
{
	t_env *new_var;
	char **args;

	if (!root || !var || !*var)
		return (1);
	
	if (var[0] && var[0] == '=')
		return (fdprintf(2, "minishell: export: `%s': not a valid identifier\n", var), 1);
	args = ft_split(var, '=');
	if (!args)
		return (fdprintf(2, "minishell: allocation error\n"), 1);
	if (args[0] && check_syntax(args[0])) 
		return (fdprintf(2, "minishell: export: `%s': not a valid identifier\n", args[0]), 1);

	clean_prev_env_var(root, args[0]);

	new_var = my_malloc(&root->list_of_list, &root->malloc_root, sizeof(t_env));
	if (!new_var)
		return (fdprintf(2, "minishell: allocation error\n"), 1);

	new_var->next = NULL; // safe default before linking
	add_var_to_list(root, &root->env, new_var, args);
	return (0);
}

// Add var to env.
// If no var, print all var of the env using env command.
int export(t_root *root, char **var)
{
	int i;

	if (!root)
		return (1);
	if (!var || !var[1])
		return (env(root->env));
	i = -1;
	while (var[++i])
		set_new_var(root, var[i]);
	return (0);
}
