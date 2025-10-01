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
int handle_quotes(t_main_data *data)
{
    char c = data->tok->input[data->tok->pos];

    if (data->tok->curr_char_type == CHAR_SINGLE_QUOTE && !data->tok->double_quote)
    {
        // Toggle single-quote state only if not inside double quotes
        data->tok->single_quote = !data->tok->single_quote;

        if (data->tok->single_quote)
        {
            // Opening single quote: start token after the quote
            if (data->tok->prev_char_type != CHAR_TEXT)
                data->tok->prev_pos = data->tok->pos + 1;
        }
        else
        {
            // Closing single quote: create token with content before the quote
            if (data->tok->pos > data->tok->prev_pos)
            {
                create_token(data, data->tok->prev_pos, data->tok->pos, TOKEN_TEXT);
            }
            data->tok->prev_pos = data->tok->pos + 1;
        }
        // Make the quote character itself behave like a separator
        data->tok->curr_char_type = CHAR_SPACE;
    }
    else if (data->tok->curr_char_type == CHAR_DOUBLE_QUOTE && !data->tok->single_quote)
    {
        // Toggle double-quote state only if not inside single quotes
        data->tok->double_quote = !data->tok->double_quote;

        if (data->tok->double_quote)
        {
            // Opening: start the token right after the quote
            if (data->tok->prev_char_type != CHAR_TEXT)
                data->tok->prev_pos = data->tok->pos + 1;
        }
        else
        {
            // Closing double quote: create token with content before the quote
            if (data->tok->pos > data->tok->prev_pos)
            {
                create_token(data, data->tok->prev_pos, data->tok->pos, TOKEN_TEXT);
            }
            data->tok->prev_pos = data->tok->pos + 1;
        }
        // Make the quote character itself behave like a separator
        data->tok->curr_char_type = CHAR_SPACE;
    }
    else if (data->tok->single_quote)
    {
        // Inside single quotes: everything is literal text
        data->tok->curr_char_type = CHAR_TEXT;
    }
    else if (data->tok->double_quote)
    {
        // Inside double quotes: keep $ as operator to allow $VAR expansion,
        // treat everything else (including spaces) as text
        if (c != '$')
            data->tok->curr_char_type = CHAR_TEXT;
        // else: keep whatever get_char_type set (CHAR_OPERATOR for '$')
    }
    return (0);
}

