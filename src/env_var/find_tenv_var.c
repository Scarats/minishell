/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_tenv_var.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:05:17 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/22 14:05:21 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// returns a pointer to the t_env node if found.
t_env	*find_tenv_var(t_env *env, char *name)
{
	t_env	*ptr;

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
