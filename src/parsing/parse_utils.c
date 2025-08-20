#include "../../minishell.h"

char check_next_char(char *str, int pos)
{
	if (!str[pos] || !str[pos + 1])
		return ('\0');
	else
		return (str[pos + 1]);
}

// Create token, add to the list, malloc.
// if first node, set prev to NULL
t_token *add_to_list(t_main_data *data, t_token *prev, int start, int end)
{
	t_token *new;

	new = my_malloc(&data->malloc_list, sizeof(t_token));
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
	new->word = ft_substr(data->tok->input, start, end - start);
	my_addtolist(&data->malloc_list, new->word);
	data->tok->token_list_size++;
	return (new);
}

// Set quote flags.
int handle_quotes(t_main_data *data)
{
    if (data->tok->curr_char_type == CHAR_DOUBLE_QUOTE)
    {
		// if (data->tok->double_quote)
			// flag double quote
        // Toggle quote state
        data->tok->double_quote = !data->tok->double_quote;
        
        // Trick the tokenizer by treating quotes as spaces (token boundaries)
        data->tok->curr_char_type = CHAR_SPACE;
    }
    else if (data->tok->curr_char_type == CHAR_SINGLE_QUOTE)
    {
        // Toggle quote state
        data->tok->single_quote = !data->tok->single_quote;
        
        // Trick the tokenizer by treating quotes as spaces (token boundaries)
        data->tok->curr_char_type = CHAR_SPACE;
    }
    else if (data->tok->double_quote || data->tok->single_quote)
    {
        // When inside quotes, treat everything as text except the closing quote
        data->tok->curr_char_type = CHAR_TEXT;
    }
    return (0);
}