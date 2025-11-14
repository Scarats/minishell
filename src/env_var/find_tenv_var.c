/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_tenv_var.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:05:17 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/14 19:48:32 by tcardair         ###   ########.fr       */
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

// Check if there is something after the $
t_token_type	no_env_var(t_main_data *data)
{
	char	next;

	next = data->tok->input[data->tok->prev_pos + 1];
	if (!next || (!ft_isalpha(next) && !ft_isdigit(next) && next != '_'
			&& next != '?' && next != '"' && next != '\''))
		return (TOKEN_TEXT);
	return (TOKEN_DOLLAR);
}
