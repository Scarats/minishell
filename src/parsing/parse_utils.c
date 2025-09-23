#include "../minishell.h"

// Removes quotes.
char *clean_string(char *input)
{
	char *output;
	int i;
	int y;

	if (!input)
		return (NULL);
	output = input;
	i = 0;
	y = 0;
	while (input[i])
	{
		if (input[i] != '"' && input[i] != '\'')
			output[y++] = input[i];
		i++;
	}
	output[y] = '\0';
	return (output);
}

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

	new = my_malloc(&data->malloc_tok, sizeof(t_token));
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
int handle_quotes(t_main_data *data)
{
    /* If we see a quote, toggle its state.
       Always treat quote characters and their contents as TEXT (transparent).
       If the quote opens and it does not follow TEXT, start the token at the first
       character inside the quote (pos + 1). */
    if (data->tok->curr_char_type == CHAR_DOUBLE_QUOTE)
    {
        /* Toggle double-quote state */
        data->tok->double_quote = !data->tok->double_quote;

        if (data->tok->double_quote)
        {
            /* Opening double-quote: if previous char wasn't text, start token inside quote */
            if (data->tok->prev_char_type != CHAR_TEXT)
                data->tok->prev_pos = data->tok->pos + 1;
        }
        /* Treat quote itself as text (no token boundary) */
        data->tok->curr_char_type = CHAR_TEXT;
    }
    else if (data->tok->curr_char_type == CHAR_SINGLE_QUOTE)
    {
        /* Toggle single-quote state */
        data->tok->single_quote = !data->tok->single_quote;

        if (data->tok->single_quote)
        {
            /* Opening single-quote: if previous char wasn't text, start token inside quote */
            if (data->tok->prev_char_type != CHAR_TEXT)
                data->tok->prev_pos = data->tok->pos + 1;
        }
        /* Treat quote itself as text (no token boundary) */
        data->tok->curr_char_type = CHAR_TEXT;
    }
    else if (data->tok->double_quote || data->tok->single_quote)
    {
        /* Inside quotes, treat everything as text */
        data->tok->curr_char_type = CHAR_TEXT;
    }
    return (0);
}

