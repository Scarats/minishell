#include "../minishell.h"

// // Removes quotes.
// char *clean_string(char *input)
// {
// 	char *output;
// 	int i;
// 	int y;

// 	if (!input)
// 		return (NULL);
// 	output = input;
// 	i = 0;
// 	y = 0;
// 	while (input[i])
// 	{
// 		if (input[i] != '"' && input[i] != '\'')
// 			output[y++] = input[i];
// 		i++;
// 	}
// 	output[y] = '\0';
// 	return (output);
// }

char check_next_char(char *str, int pos)
{
	if (!str[pos] || !str[pos + 1])
		return ('\0');
	else
		return (str[pos + 1]);
}

// Create token, add to the list, malloc.
// if first node, set prev to NULL
t_token *add_to_list(t_main_data *data, t_token *prev)
{
	t_token *new;

	new = my_malloc(&data->root->list_of_list, &data->malloc_tok, sizeof(t_token));
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
	data->tok->last_token = new;
	new->next_token = NULL;

	data->tok->token_list_size++;
	return (new);
}

// Set quote flags.
int handle_quotes(t_tokenizer *tok, t_main_data *data)
{
    char c;
	
	c = data->tok->input[tok->pos];
	if (c == '\'' && !tok->double_quote)
	{
		tok->single_quote = !tok->single_quote;
		tok->curr_char_type = CHAR_SPACE;
	}
	else if (c == '"' && !tok->single_quote)
	{
		tok->double_quote = !tok->double_quote;
		tok->curr_char_type = CHAR_SPACE;
	}
    else if (tok->double_quote && c != '$')
        tok->curr_char_type = CHAR_TEXT;
	else if (tok->single_quote)
		tok->curr_char_type = CHAR_TEXT;
    return (0);
}

