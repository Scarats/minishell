/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:00:35 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/22 15:01:31 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_numeric(char *str)
{
	int	i;
	int	found_digit;

	i = 0;
	found_digit = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
			|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ft_isdigit(str[i]))
	{
		found_digit = 1;
		i++;
	}
	while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
			|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r'))
		i++;
	return (found_digit && str[i] == '\0');
}

static void	perform_exit(t_main_data *data, int exit_code)
{
	if (data->in_child)
		exit(exit_code & 255);
	cleanup(data, data->root);
	ft_lstclear(&data->root->list_of_list, NULL);
	my_free(&data->root->malloc_root);
	exit(exit_code & 255);
}

int	exit_builtin(t_node *node, t_main_data *data)
{
	int	exit_code;

	if (!data->in_child)
		ft_printf("exit\n");
	exit_code = data->root->last_exit_status;
	if (node->cmd_argv[1])
	{
		if (!is_numeric(node->cmd_argv[1]))
		{
			fdprintf(2, "minishell: exit: %s: numeric argument required\n",
				node->cmd_argv[1]);
			exit_code = 2;
		}
		else
		{
			exit_code = ft_atoi(node->cmd_argv[1]);
			if (node->cmd_argv[2])
				return ((void)fdprintf(2,
						"minishell: exit: too many arguments\n"), 1);
		}
	}
	perform_exit(data, exit_code);
	return (0);
}
