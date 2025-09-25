#include "../minishell.h"

// remove_token: unlink a token from a doubly-linked list
void remove_token(t_token **head, t_token *t)
{
    if (!t)
		return;
    if (t->prev_token)
		t->prev_token->next_token= t->next_token;
    else
		*head = t->next_token;
    if (t->next_token)
		t->next_token->prev_token = t->prev_token;
}

// Determine if the word is a command, argument, filename etc...
// It will be done according to the previous tokens created.
t_token_type get_word_type(t_token *tok)
{
	t_token_type prev_type;

	// It's the first node, then COMMAND
	if (tok->prev_token == NULL)
		return (TOKEN_CMD);
	prev_type = tok->prev_token->type;
	if (prev_type == TOKEN_REDIRECT_OUT || prev_type == TOKEN_REDIRECT_IN || prev_type == TOKEN_APPEND || prev_type == TOKEN_HEREDOC)
		return (TOKEN_FILE);
	else if (prev_type == TOKEN_PIPE || prev_type == TOKEN_AND_AND || prev_type == TOKEN_OR || prev_type == TOKEN_LPAREN || prev_type == TOKEN_RPAREN)
		return (TOKEN_CMD);
	else if (prev_type == TOKEN_DOLLAR)
		return (TOKEN_ENV_VAR);
	else
		return (TOKEN_ARGUMENT);
}

// Create token, add them to the list and add type.
int create_token(t_main_data *data, int start, int end, t_token_type type)
{
	t_token *tok;
	bool text;
	char *word;

    text = false;
    word = NULL;
    if (type == TOKEN_SPACE)
        return (0);
    tok = add_to_list(data, data->tok->last_token);
    if (!tok)
        return (1);
    if (type == TOKEN_TEXT)
    {
        type = get_word_type(tok);
        text = true;
    }
    tok->type = type;
    if (text || tok->type == TOKEN_ENV_VAR)
        word = ft_substr(data->tok->input, start, end - start);
    if (tok->type == TOKEN_ENV_VAR)
    {
        tok->word = get_env_var(((t_root *)data->root)->env, word);
        if (tok->prev_token && tok->prev_token->type == TOKEN_DOLLAR)
            remove_token(&data->tok->token_list, tok->prev_token);
        tok->type = get_word_type(tok);
    }
    else if (text)
        tok->word = word;
    // Track the temp substring in all cases where we allocated it
    if (word)
        my_addtolist(&data->malloc_tok, word);
    tok->word = clean_string(tok->word);
    return (0);
}

t_char_type get_char_type(char c)
{
	if (c == ' ')
		return (CHAR_SPACE);
	else if (c == '|' || c == '&' || c == '<' || c == '>' || c == '$')
		return (CHAR_OPERATOR);
	else if (c == '(' || c == ')')
		return (CHAR_PARENTHESIS);
	else if (c == '\'')
		return (CHAR_SINGLE_QUOTE);
	else if (c == '"')
		return (CHAR_DOUBLE_QUOTE);
	return (CHAR_TEXT);
}

t_token_type get_tok_type(char c, char next)
{
	if (c == '&' && next == '&')
		return (TOKEN_AND_AND);
	else if (c == '|' && next == '|')
		return (TOKEN_OR);
	else if (c == '<' && next == '<')
		return (TOKEN_HEREDOC);
	else if (c == '>' && next == '>')
		return (TOKEN_APPEND);
	else if (c == '$')
		return (TOKEN_DOLLAR);
	else if (c == '&')
		return (TOKEN_AND);
	else if (c == '|')
		return (TOKEN_PIPE);
	else if (c == '<')
		return (TOKEN_REDIRECT_IN);
	else if (c == '>')
		return (TOKEN_REDIRECT_OUT);
	else if (c == '(')
		return (TOKEN_LPAREN);
	else if (c == ')')
		return (TOKEN_RPAREN);
	else if (c == ' ')
		return (TOKEN_SPACE);
	return (TOKEN_TEXT);
}

int tokenizer(t_main_data *data)
{
    t_token_type tok_type;

    tok_type = TOKEN_NULL;
    while (data->tok->pos < data->tok->length)
    {
        data->tok->curr_char_type = get_char_type(data->tok->input[data->tok->pos]);
        handle_quotes(data);

        // Force-split consecutive parentheses: "((" and "))"
        if (data->tok->curr_char_type == CHAR_PARENTHESIS
            && data->tok->prev_char_type == CHAR_PARENTHESIS
            && data->tok->pos > data->tok->prev_pos)
        {
            tok_type = get_tok_type(
                data->tok->input[data->tok->prev_pos],
                check_next_char(data->tok->input, data->tok->prev_pos));
            create_token(data, data->tok->prev_pos, data->tok->prev_pos + 1, tok_type);
            data->tok->prev_pos = data->tok->pos;
        }

        if (data->tok->curr_char_type != data->tok->prev_char_type)
        {
            if (data->tok->prev_char_type != CHAR_SPACE)
            {
                // && data->tok->pos > data->tok->prev_pos)  // avoid zero-length tokens
                tok_type = get_tok_type(
                    data->tok->input[data->tok->prev_pos],
                    check_next_char(data->tok->input, data->tok->prev_pos));
                create_token(data, data->tok->prev_pos, data->tok->pos, tok_type);
            }
            data->tok->prev_pos = data->tok->pos;
        }
        data->tok->prev_char_type = data->tok->curr_char_type;
        data->tok->pos++;
    }
    if (data->tok->prev_char_type != CHAR_SPACE && data->tok->prev_pos < data->tok->pos)
    {
        tok_type = get_tok_type(data->tok->input[data->tok->prev_pos],
                                check_next_char(data->tok->input, data->tok->prev_pos));
        create_token(data, data->tok->prev_pos, data->tok->pos, tok_type);
    }
    return (0);
}

// Turn the token linked list in an array, easier for AST.
int list_to_array(t_main_data *data, t_token *token_list, int size)
{
    t_token *curr_tok;
    t_root *root;
    int i;
    int real_size;

    // Recompute size after possible removals (e.g., '$' tokens)
    real_size = 0;
    curr_tok = token_list;
    while (curr_tok)
    {
        real_size++;
        curr_tok = curr_tok->next_token;
    }
    size = real_size;
    data->tok->token_list_size = real_size;

    i = 0;
    root = data->root;
    data->tok->token_array = my_malloc(&root->list_of_list, &data->malloc_tok,
                                       sizeof(t_token) * (size > 0 ? size : 1));
    if (!data->tok->token_array && size > 0)
        return (1);

    curr_tok = token_list;
    while (curr_tok && i < size)
    {
        data->tok->token_array[i] = *curr_tok;
        i++;
        curr_tok = curr_tok->next_token;
    }
    return (0);
}

// Parse the input.
// Tokenize it, then create a binary tree.
int parser(t_main_data *data)
{
	if (tokenizer(data))
		return (1);
	if (list_to_array(data, data->tok->token_list, data->tok->token_list_size))
		return (1);
	data->node = build_tree(data, data->tok->token_array, data->tok->token_list_size, 0);
	if (!data->node)
		return (1);
	return (0);
}


