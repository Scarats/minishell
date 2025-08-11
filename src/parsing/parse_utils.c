#include "../../minishell.h"

char check_next_char(char *str, int pos)
{
	if (!str[pos] || !str[pos + 1])
		return ('\0');
	else
		return (str[pos + 1]);
}

// if first node, set prev to NULL
t_token *add_to_list(t_main_data *data,	t_token *prev, int start, int end)
{
	t_token *new;
	
	new = my_malloc(data->malloc_list, sizeof(t_token));
	if (!new)
		return (NULL);
	if (prev == NULL)
	{
		new->prev_token = NULL;
		data->tok->token_list = new;
	}
	else
	{
		prev->next_token = new;
		new->prev_token = prev;
	}
	new->next_token = NULL;
	new->word = ft_substr(data->tok->input, start, end - start);
	my_addtolist(data->malloc_list, new->word);
	data->tok->token_list_size++;
	return (new);
}
