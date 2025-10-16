#include "../minishell.h"

char	**t_env_to_char_arr(t_root *root, t_env *list)
{
	t_env	*ptr;
	char	**env;
	char	*tmp;
	int		i;

	ptr = list;
	i = 0;
	while (ptr && ++i)
		ptr = ptr->next;
	env = my_malloc(&root->list_of_list, &root->data->malloc_tree,
			sizeof(char *) * (i + 1));
	ptr = list;
	i = 0;
	while (ptr)
	{
		tmp = ft_strjoin(ptr->name, "=");
		env[i] = ft_strjoin(tmp, ptr->value);
		my_addtolist(&root->data->malloc_tree, env[i++]);
		free(tmp);
		ptr = ptr->next;
	}
	env[i] = NULL;
	return (env);
}

// Duplicate a t_env linked list.
// New memory is allocated and added to the malloc_list,
// t_env *copy_env(t_list **malloc_list, t_env *env)
// {
//     t_env *new_head;
//     t_env *new_tail;
//     t_env *node;

//     if (!env || !malloc_list)
//         return (NULL);
//     new_head = NULL;
//     new_tail = NULL;
//     while (env != NULL)
//     {
//         node = my_malloc(NULL, &malloc_list, sizeof(t_env));
//         if (!node)
//             return (NULL);
//         node->name = my_strdup(&malloc_list, env->name);
//         node->value = my_strdup(&malloc_list, env->value);
//         if (!node->name || !node->value)
//             return (NULL);
//         if (!new_head)
//             new_head = node;
//         else
//             new_tail->next = node;
//         new_tail = node;
//         env = env->next;
//     }
//     return (new_head);
// }
