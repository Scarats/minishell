/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:47:45 by tcardair          #+#    #+#             */
/*   Updated: 2025/09/30 17:22:20 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*loop_find_bin(char **path, char *slash_bin)
{
	char	*tmp;
	size_t	i;

	i = 0;
	if (!path || !*path || !slash_bin)
		return (NULL);
	while (path[i])
	{
		if (path[i][0] == '\0')
			tmp = ft_strjoin(".", slash_bin);
		else
			tmp = ft_strjoin(path[i], slash_bin);
		if (!tmp)
			break ;
		if (access(tmp, X_OK) == 0)
			return (tmp);
		free(tmp);
		i++;
	}
	return (NULL);
}

// Will try to find the given bin name in PATH.
// Returns an allocated path string or NULL. Caller must free.
char	*find_bin(char *bin)
{
	char	*env;
	char	*tmp;
	char	*slash_bin;
	char	**path;

	if (!bin || !*bin)
		return (NULL);
	env = getenv("PATH");
	if (!env)
		return (NULL);
	path = ft_split(env, ':');
	if (!path)
		return (NULL);
	slash_bin = ft_strjoin("/", bin);
	if (!slash_bin)
		return (free_2d_array((void **)path), NULL);
	tmp = loop_find_bin(path, slash_bin);
	if (tmp != NULL)
		return (free(slash_bin), free_2d_array((void **)path), tmp);
	return (free(slash_bin), free_2d_array((void **)path), NULL);
}
