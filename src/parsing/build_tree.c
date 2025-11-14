/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcardair <tcardair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 18:50:28 by tcardair          #+#    #+#             */
/*   Updated: 2025/11/14 20:00:20 by tcardair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Handle redirection token and create redir node
int	handle_redirection(t_cmd_builder *builder, int *i)
{
	t_redir	*curr_redir;

	if (*i + 1 >= builder->size || builder->tok_array[*i
			+ 1].type != TOKEN_FILE)
		return (1);
	curr_redir = add_redirection(builder->data, builder->node,
			builder->tok_array[*i].type);
	(*i)++;
	curr_redir->filename = my_strdup(&builder->data->malloc_tree,
			builder->tok_array[*i].word);
	return (0);
}

// Add command or argument to cmd_argv
void	add_cmd_or_arg(t_cmd_builder *builder, int i, int *j)
{
	if (builder->tok_array[i].type == TOKEN_CMD
		|| builder->tok_array[i].type == TOKEN_ARGUMENT
		|| builder->tok_array[i].type == TOKEN_TEXT
		|| builder->tok_array[i].type == TOKEN_ENV_VAR)
	{
		builder->node->cmd_argv[*j] = my_strdup(&builder->data->malloc_tree,
				builder->tok_array[i].word);
		(*j)++;
	}
}

// Process all tokens and build cmd_argv
int	process_tokens(t_cmd_builder *builder, int cmd_argc)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (++i < builder->size)
	{
		if (is_redir(builder->tok_array[i].type))
		{
			if (handle_redirection(builder, &i))
				return (1);
		}
		else if (j < cmd_argc)
			add_cmd_or_arg(builder, i, &j);
	}
	builder->node->cmd_argv[j] = NULL;
	return (0);
}

// Create the excve argv, add cmd and args to it, set redirections.
int	create_node_cmd(t_main_data *data, t_token *tok_array, t_node *node,
		int size)
{
	t_cmd_builder	builder;
	int				cmd_argc;
	t_root			*root;

	root = data->root;
	builder.data = data;
	builder.tok_array = tok_array;
	builder.node = node;
	builder.size = size;
	cmd_argc = get_cmd_argc(tok_array, size);
	node->cmd_argv = my_malloc(&root->list_of_list, &data->malloc_tree,
			sizeof(char *) * (cmd_argc + 2));
	if (!node->cmd_argv)
		return (1);
	if (process_tokens(&builder, cmd_argc))
		return (1);
	if (node->cmd_argv[0] && is_builtin(node->cmd_argv[0]))
		node->builtin = true;
	return (0);
}

// While size, add token, check its type and add it to the node.
t_node	*create_node(t_main_data *data, t_token *tok_array, t_node_type type,
		int size)
{
	t_node	*node;
	t_root	*root;

	root = data->root;
	node = my_malloc(&root->list_of_list, &data->malloc_tree, sizeof(t_node));
	node->type = type;
	node->create_subshell = false;
	node->builtin = false;
	node->pipefd[0] = -1;
	node->pipefd[1] = -1;
	node->left_pid = -1;
	node->right_pid = -1;
	node->input_fd = -1;
	node->output_fd = -1;
	if (type == NODE_COMMAND && tok_array)
		create_node_cmd(data, tok_array, node, size);
	return (node);
}
