/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:01:50 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/23 13:32:43 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup(t_main_data *data, t_root *root)
{
	my_free(&data->malloc_tok);
	my_free(&data->malloc_tree);
	my_multi_free(&root->list_of_list);
	write_history(".minishell_history");
}

void	handle_empty_input(char **line)
{
	free(*line);
	*line = NULL;
}

void	handle_eof(char **line)
{
	write(STDOUT_FILENO, "exit\n", 5);
	free(*line);
	*line = NULL;
}

void	cleanup_after_command(t_main_data *data, char **line)
{
	my_free(&data->malloc_tok);
	my_free(&data->malloc_tree);
	data->node = NULL;
	g_stop_flag = 0;
	free(*line);
	*line = NULL;
}
