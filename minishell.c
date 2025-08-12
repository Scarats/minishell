#include "minishell.h"

/* int init(t_main_data *data)
{
	ft_memset(data, 0, sizeof(data));
	data->input = "This is a test";
	data->tok->quote_state = QUOTE_NONE;
	data->tok->length = ft_strlen(data->tok->input);
}

int main()
{
	t_main_data data;
	t_token curr;

	init(&data);
	traverse_tree(data.node, &data);
	while (data.tok->token_list->next_token != NULL)
	{
		printf();
	}
	return(0);
} */


int init(t_main_data *data)
{
    // Allocate memory for the token structure
    data->tok = malloc(sizeof(t_tokenizer));
    if (!data->tok)
        return (1);
    
    data->malloc_list = malloc(sizeof(t_token));
    if (!data->malloc_list)
    {
        free(data->tok);
        return (1);
    }
    
    // Initialize values
    ft_memset(data->tok, 0, sizeof(t_tokenizer));
    ft_memset(data->malloc_list, 0, sizeof(t_token));
    
    // Set input string
    data->tok->input = "echo hello | grep h > output.txt && ls -la";
    data->tok->length = ft_strlen(data->tok->input);
    data->tok->pos = 0;
    data->tok->prev_pos = 0;
    data->tok->quote_state = QUOTE_NONE;
    data->tok->token_list = NULL;
    data->tok->last_token = NULL;
    data->tok->token_list_size = 0;
    data->tok->prev_char_type = CHAR_SPACE;
    
    // Initialize previous character type to avoid initial issues
    
    return (0);
}

void print_token_type(t_token_type type)
{
    char *type_str;
    
    switch (type)
    {
        case TOKEN_NULL: type_str = "NULL"; break;
        case TOKEN_TEXT: type_str = "TEXT"; break;
        case TOKEN_SPACE: type_str = "SPACE"; break;
        case TOKEN_PIPE: type_str = "PIPE"; break;
        case TOKEN_AND: type_str = "AND"; break;
        case TOKEN_OR: type_str = "OR"; break;
        case TOKEN_AND_AND: type_str = "AND_AND"; break;
        case TOKEN_REDIRECT_IN: type_str = "REDIRECT_IN"; break;
        case TOKEN_REDIRECT_OUT: type_str = "REDIRECT_OUT"; break;
        case TOKEN_APPEND: type_str = "APPEND"; break;
        case TOKEN_HEREDOC: type_str = "HEREDOC"; break;
        case TOKEN_LPAREN: type_str = "LPAREN"; break;
        case TOKEN_RPAREN: type_str = "RPAREN"; break;
        case TOKEN_SPARAM: type_str = "SPARAM"; break;
        case TOKEN_CMD: type_str = "CMD"; break;
        case TOKEN_ARGUMENT: type_str = "ARGUMENT"; break;
        case TOKEN_FILE: type_str = "FILE"; break;
        default: type_str = "UNKNOWN"; break;
    }
    
    printf("%-12s", type_str);
}

int main()
{
    t_main_data data;
    t_token *current;
    int token_count = 0;
    
    if (init(&data) != 0)
    {
        printf("Initialization failed.\n");
        return (1);
    }
    
    printf("Input: \"%s\"\n\n", data.tok->input);
    
    // Parse input into tokens
    if (parser(&data) != 0)
    {
        printf("Parsing failed.\n");
        return (1);
    }
    
    // Print header
    printf("%-4s | %-12s | %-s\n", "No.", "Type", "Value");
    printf("----------------------------------\n");
    
    // Print all tokens
    current = data.tok->token_list;
    while (current)
    {
        printf("%-4d | ", ++token_count);
        print_token_type(current->type);
        printf(" | \"%s\"\n", current->word);
        current = current->next_token;
    }
    
    printf("\nTotal tokens: %d\n", token_count);
    
    // If you have a tree structure, traverse and print it
    if (data.node)
    {
        printf("\nTree traversal:\n");
        //traverse_tree(data.node, &data);
    }
    
    // Free memory - implement your cleanup function
    // cleanup_memory(&data);
    
    return (0);
}
