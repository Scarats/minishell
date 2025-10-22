/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_last_exit_status.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:58:18 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/22 14:58:53 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_last_exit_status_var(t_root *root, int status)
{
	char	*num;
	char	*kv;
	char	*args[2];

	num = ft_itoa(status);
	if (!num)
		return (fdprintf(2, "minishell: allocation error\n"), 1);
	kv = ft_strjoin("?=", num);
	if (!kv)
		return (fdprintf(2, "minishell: allocation error\n"), 1);
	args[0] = kv;
	args[1] = NULL;
	return (export(root, args));
}
