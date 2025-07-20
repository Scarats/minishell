/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:47:45 by tcardair          #+#    #+#             */
/*   Updated: 2025/07/20 18:16:55 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Will try to find the given bin name in "/bin/", "/usr/bin/", "/usr/local/bin/".
// Return the path to the binary.
// If not found, return NULL.
// Memory is allocated. Do not forget to add it to the garbadge collector.
char *find_bin(char *bin)
{
    char *tmp;

    tmp = ft_strjoin("/bin/", bin);
    if (!check_access(tmp, 1, 0, 1, 1))
        return (tmp);
    free(tmp);

    tmp = ft_strjoin("/usr/bin/", bin);
    if (!check_access(tmp, 1, 0, 1, 1))
        return (tmp);
    free(tmp);

    tmp = ft_strjoin("/usr/local/bin/", bin);
    if (!check_access(tmp, 1, 0, 1, 1))
        return (tmp);
    free(tmp);

    return (NULL);
}