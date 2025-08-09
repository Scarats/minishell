#include "../../minishell.h"

// Assign type as a void, so it can be done directly in a return(assign_type, 1);
// action:
// 	1 = assign the given type.
//	2 = assign the duplicate type (&&, ||, <<, >>) of the current sign.
//		increases the tok->pos to match the new size.
void assign_type(t_token_type *curr_tok_type, t_token_type type, int action)
{
	if (action == 1)
	{
		curr_tok_type = type;
		return;
	}
	else if (curr_tok_type == TOKEN_AND)
}

// Will check each char if it is a special char such as:
// < > << >> $ "" '' ...
// Assign the corresponding type to curr_tok_type
// returns 0 if not detected (or it's a space)
// returns 1 if detected
int check_char(char c, t_main_data *data)
{
	// If space, not in a word. But in a word and space, then end of word.
	if (c == " ")
	{
		if (data->tok->in_word = 0)
		{
			data->tok->prev_pos++;
			return (assign_type(data->tok->curr_tok_type, TOKEN_SPACE, 1), 0);
		}
		data->tok->in_word = 0;
		return (assign_type(data->tok->curr_tok_type, TOKEN_SPACE, 1), 1);
	}
	else if (c == "\0")
		return (assign_type(data->tok->curr_tok_type, TOKEN_NULL, 1), 1);
	// else if ...
}

// Create a new token in the list from tok->prev_pos to tok->pos - 1
// Increases token_list_size and prev_pos
// Check the next char since it can be &&, ||, <<, >>, $something etc...
int create_token(t_main_data *data)
{
	// If in_word and here, it means there is a redir, we have to create 2 tokens in a row.
	if (data->tok->in_word == 1 && data->tok->curr_tok_type != TOKEN_NULL)
	{
		// Create the previous word
		// Create the current word (<< >> && ||)
	}
	data->tok->prev_pos = data->tok->pos;
}

// Make each word a token.
// Add a type to each token.
// Be careful with spaces to ignore them.
int tokenizer(t_main_data *data)
{
	while (data->tok->pos < data->tok->length)
	{
		data->tok->prev_type = data->tok->curr_tok_type;
		if (check_char(data->tok->input[data->tok->pos], data))
		{
			if (data->tok->in_word == 1)
				create_token(data);
		}

		data->tok->pos++;
	}
}

// Parse the input.
// Tokenize it, then create a binary tree.
int parser(t_main_data *data)
{
	if (tokenizer(data))
		return (1);
	if (build_tree(data))
		return (1);
	return (0);
}
