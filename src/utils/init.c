/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 11:23:45 by tcardair          #+#    #+#             */
/*   Updated: 2025/10/23 15:44:48 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	create_prompt(char *prompt, size_t size)
{
	char	hostname[64];
	char	username[64];

	hostname[0] = '\0';
	username[0] = '\0';
	gethostname(hostname, sizeof(hostname) - 1);
	getlogin_r(username, sizeof(username) - 1);
	ft_bzero(prompt, size);
	ft_strlcat(prompt, BLUE, size);
	ft_strlcat(prompt, username, size);
	ft_strlcat(prompt, RESET "@" GREEN, size);
	ft_strlcat(prompt, hostname, size);
	ft_strlcat(prompt, RESET ":" PURPLE "minishell" RESET "> ", size);
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
	create_prompt(prompt, prompt_size);
	handle_signals();
	if (init(root))
	{
		fdprintf(2, "Initialization failed\n");
		return (1);
	}
	read_history(".minishell_history");
	return (0);
}
