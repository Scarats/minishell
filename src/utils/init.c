/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadeikal <aadeikal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 11:23:45 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/06 15:59:05 by aadeikal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_host_from_env(t_root *root)
{
	char *hostname;
	char *host_env;
	int i;
	int len;
	int y;

	i = 0;
	host_env = get_env_var(root->env, "SESSION_MANAGER");
	len = ft_strlen(host_env);
	hostname = my_malloc(&root->list_of_list, &root->malloc_root, sizeof(char) * 7);
	if (!hostname || !host_env)
		return (NULL);
	hostname[6] = '\0';
	while (i < len)
	{
		if(host_env[i] == '/')
			break ;
		i++;	
	}
	i++;
	y = 0;
	while (host_env[i] && host_env[i] != '.')
		hostname[y++] = host_env[i++];
	return (hostname);
} 

void	create_prompt(char *prompt, size_t size, t_root *root)
{
	char	*username;
	char	*hostname;

	username = get_env_var(root->env, "LOGNAME");
	hostname = get_host_from_env(root);
	get_host_from_env(root);
	ft_bzero(prompt, size);
	//ft_strlcat(prompt, RESET, size);
	if (username && hostname)
	{
		ft_strlcat(prompt, username, size);
		ft_strlcat(prompt, "@", size);
		ft_strlcat(prompt, hostname, size);
		ft_strlcat(prompt, ":minishell> ", size);
	}
	else
		ft_strlcat(prompt, PURPLE "minishell> ", size);
}

int	init(t_root *root)
{
	t_main_data	*data;

	data = root->data;
	ft_memset(data, 0, sizeof(*data));
	data->root = root;
	data->tok = my_malloc(&root->list_of_list, &root->malloc_root,
			sizeof(t_tokenizer));
	if (!data->tok)
		return (1);
	data->tok->prev_char_type = CHAR_SPACE;
	data->tok->curr_char_type = CHAR_NULL;
	data->tok->double_quote = false;
	data->tok->single_quote = false;
	data->node = NULL;
	data->malloc_tok = NULL;
	data->malloc_tree = NULL;
	data->error = 0;
	data->in_child = false;
	return (0);
}

void	reset_tokenizer_for_line(t_tokenizer *tok, char *line)
{
	tok->input = line;
	tok->length = (int)ft_strlen(line);
	tok->pos = 0;
	tok->prev_pos = 0;
	tok->prev_char_type = CHAR_SPACE;
	tok->curr_char_type = CHAR_NULL;
	tok->double_quote = false;
	tok->single_quote = false;
	tok->token_list = NULL;
	tok->last_token = NULL;
	tok->token_list_size = 0;
}

int	initialize_shell(t_root *root, char *prompt, size_t prompt_size)
{
	create_prompt(prompt, prompt_size, root);
	handle_signals();
	if (init(root))
	{
		fdprintf(2, "Initialization failed\n");
		return (1);
	}
	read_history(".minishell_history");
	return (0);
}
