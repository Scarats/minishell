#include "../../minishell.h"

// Assign type as a void, so it can be done directly in a return(assign_type, 1);
// action:
// 	1 = assign the given type.
//	2 = assign the duplicate type (&&, ||, <<, >>) of the current sign.
void assign_type(t_token_type *curr_tok_type, t_token_type type, int action)
{
	curr_tok_type = type;
}

// Will check each char if it is a special char such as:
// < > << >> $ "" '' ...
// Assign the corresponding type to curr_tok_type
// returns 0 if not detected (or it's a space)
// returns 1 if detected
int check_char(char c, t_main_data *data)
{
	if (c == " ")
	{
		data->tok->pos++;
		data->tok->prev_pos++;
		return (0);
	}
	else if (c == "<")
		return (assign_type(data->tok->curr_tok_type, TOKEN_REDIRECT_OUT), 1);
	// else if ...
}

// Create a new token in the list from tok->prev_pos to tok->pos
// Increases token_list_size and prev_pos
// Check the next char since it can be &&, ||, <<, >>, $something etc...
int create_token(t_main_data *data)
{
	// Check if the next char is the same
	if (data->tok->input[data->tok->pos + 1] && (data->tok->input[data->tok->pos + 1] == data->tok->input[data->tok->pos]))
		s

			data->tok->prev_pos++;
}

// Make each word a token.
// Add a type to each token.
// Be careful with spaces to ignore them.
int tokenizer(t_main_data *data)
{
	while (data->tok->pos < data->tok->length)
	{
		if (check_char(data->tok->input[data->tok->pos], data))
			create_token(data);

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
