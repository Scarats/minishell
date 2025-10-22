/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:04:44 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/22 14:04:44 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
