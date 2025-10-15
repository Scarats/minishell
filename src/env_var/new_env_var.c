#include "../minishell.h"

// Duplicate a t_env linked list.
// New memory is allocated and added to the malloc_list,
t_env *new_env_var(t_list *malloc_list, t)
{
	t_env *new_var;

	if (!malloc_list || !env)
		return (NULL);
	new_var = my_malloc(&malloc_list, sizeof(t_env));
	new
}

