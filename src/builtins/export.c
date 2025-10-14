#include "../minishell.h"

int check_syntax(char *name)
{
	int size;

	size = ft_strlen(name);
	while (size-- >= 0)
	{
		if (!(ft_isalpha(name[size] || name[size] == '_')))
			return (1);
	}
	return (0);
}

// Add var to env.
// If no var, print all var of the env using env command.
int export(t_root *root, char *var)
{
	t_env *new_var;
	t_env *ptr;
	char **args;
	char **name;

	if (!root)
		return (1);
	if (!var)
		return (env(root->env));
	args = NULL;
	name = NULL;

	// Split the name and the values.
	args = ft_split(var, '=');

	// Check if the name is valid.
	if (args[0] && check_syntax(args[0]))
		return (fdprintf(2, "minishell:"));

	// Remove already existing variables with this name.
	name = malloc(sizeof(char *) * 2);
	name[0] = args[0];
	name[1] = NULL;
	unset(&root->env, name);
	free(name);

	new_var = my_malloc(&root->list_of_list, &root->malloc_root, sizeof(t_env));

	// Go to the last var of the list.
	ptr = root->env;
	while (ptr->next != NULL)
		ptr = ptr->next;

	// Add the new var to the end of the list.
	ptr->next = new_var;
	my_array_addtolist(&root->malloc_root, (void **)args);
	new_var->exported = true;
	new_var->name = args[0];
	new_var->value = args[1];
	new_var->next = NULL;
	return (0);
}
