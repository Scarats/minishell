/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:41:55 by aadeikal          #+#    #+#             */
/*   Updated: 2025/11/21 15:42:36 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup_parent_fds(t_node *node)
{
	if (node->input_fd != -1 && node->input_fd != STDIN_FILENO)
		close(node->input_fd);
	if (node->output_fd != -1 && node->output_fd != STDOUT_FILENO)
		close(node->output_fd);
}

int	get_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		return (128 + WTERMSIG(status));
	}
	return (1);
}