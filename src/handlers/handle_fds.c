/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 23:02:12 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/17 23:15:17 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int backup_fds(t_fd_backup *fd)
{
	fd->in = dup(STDIN_FILENO);
	fd->out = dup(STDOUT_FILENO);
	if (fd->in == -1 || fd->out == -1)
	{
		if (fd->in != -1)
			close(fd->in);
		if (fd->out != -1)
			close(fd->out);
		return (1);
	}
	return (0);
}

int reset_fds(t_fd_backup *fd)
{
	int error;

	error = 0;
	if (dup2(fd->in, STDIN_FILENO) == -1)
		error = 1;
	if (dup2(fd->out, STDOUT_FILENO) == -1)
		error = 1;
	close(fd->in);
	close(fd->out);
	return (error);
}