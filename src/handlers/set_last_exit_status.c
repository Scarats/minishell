/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_last_exit_status.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:58:18 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/30 22:48:54 by aadeikal         ###   ########.fr       */
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

int set_last_exit_status_from_wait(t_root *root, int status)
{
    if (WIFEXITED(status))
    {
        root->last_exit_status = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status))
    {
        int sig = WTERMSIG(status);
        root->last_exit_status = 128 + sig;
        if (sig == SIGQUIT)
            write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
    }
    else
    {
        root->last_exit_status = 1;
    }
    return root->last_exit_status;
}
