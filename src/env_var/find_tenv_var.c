#include "../minishell.h"

// returns a pointer to the t_env node if found.
t_env *find_tenv_var(t_env *env, char *name)
{
	t_env *ptr;

	if (!env || !name)
		return (NULL);
	ptr = env;
	while (ptr)
	{
		if (!ft_strcmp(name, ptr->name))
			return (ptr);
		ptr = ptr->next;
	}
	return (NULL);
}