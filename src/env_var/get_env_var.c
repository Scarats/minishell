/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:05:40 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/22 17:48:01 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Find the var's value in the shell's environment.
char	*get_env_var(t_env *env, char *target)
{
	t_env	*ptr;

	if (!env || !target)
		return (NULL);
	ptr = env;
	while (ptr)
	{
		if (!ft_strcmp(target, ptr->name))
			return (ptr->value);
		ptr = ptr->next;
	}
	return (NULL);
}

// Split at the first =
char	**split_env_var(t_root *root, char *env)
{
	char	**env_var;
	int		i;
	int		y;

	if (!env || !root)
		return (NULL);
	env_var = my_malloc(&root->list_of_list, &root->malloc_root, sizeof(char *)
			* 3);
	i = 0;
	y = -1;
	while (env[i] && env[i] != '=')
		i++;
	if (!env[i])
	{
		env_var[0] = my_malloc(&root->list_of_list, &root->malloc_root, i + 1);
		if (!env_var[0])
			return (NULL);
		ft_memcpy(env_var[0], env, i);
		env_var[0][i] = '\0';
		env_var[1] = NULL;
		env_var[2] = NULL;
		return (env_var);
	}
	env_var[0] = my_malloc(&root->list_of_list, &root->malloc_root, sizeof(char)
			* (i + 1));
	while (++y < i)
		env_var[0][y] = env[y];
	env_var[0][y] = '\0';
	env_var[1] = my_malloc(&root->list_of_list, &root->malloc_root, sizeof(char)
			* ((ft_strlen(env) - i) + 1));
	y = 0;
	i++;
	while (env[i])
		env_var[1][y++] = env[i++];
	env_var[1][y] = '\0';
	env_var[2] = NULL;
	return (env_var);
}

// Create a t_env struct from a char **, spliting NAME=value.
t_env	*set_env_var_list(t_root *root, char **env)
{
	t_env_utils	utils;

	if (!env)
		return (NULL);
	utils.head = NULL;
	utils.tail = NULL;
	utils.i = -1;
	while (env[++utils.i])
	{
		utils.tmp = split_env_var(root, env[utils.i]);
		if (!utils.tmp || utils.tmp[0] == NULL || utils.tmp[1] == NULL)
			return (free_2d_array((void **)utils.tmp), NULL);
		utils.node = my_malloc(&root->list_of_list, &root->malloc_root,
				sizeof(t_env));
		utils.node->name = utils.tmp[0];
		utils.node->value = utils.tmp[1];
		utils.node->next = NULL;
		if (!utils.head)
			utils.head = utils.node;
		else
			utils.tail->next = utils.node;
		utils.tail = utils.node;
	}
	return (utils.head);
}
