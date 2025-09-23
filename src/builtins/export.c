#include "../minishell.h"

int export(t_root *root, char *var)
{
	t_env *new_var;
	char **args;

	if (!root)
		return (1);
	args = NULL;
	new_var = my_malloc(&root->malloc_root, sizeof(t_env));
	while (root->env->next != NULL)
		root->env = root->env->next;
	root->env->next = new_var;
	args = ft_split(var, '=');
	new_var->exported = true;
	new_var->name = args[0];
	new_var->value = args[1];
	new_var->next = NULL;
	return (0);
}
