#include "minishell.h"

int init(t_main_data *data)
{
    // Allocate memory for the token structure
    data->tok = malloc(sizeof(t_tokenizer));
    if (!data->tok)
        return (1);
    
    // Initialize values
    ft_memset(data->tok, 0, sizeof(t_tokenizer));
    
    // Set input string
    data->tok->input = "echo hello | grep h > output.txt && ls -la $test";
    data->tok->input = "for file \"in\" $(find /etc -type f 2>/dev/null | head -n 10) && do echo";
    data->tok->length = ft_strlen(data->tok->input);
    data->tok->pos = 0;
    data->tok->prev_pos = 0;
    data->tok->token_list = NULL;
    data->tok->last_token = NULL;
    data->tok->token_list_size = 0;
    
    // Initialize other data fields
    data->node = NULL;
    data->malloc_list = NULL;
    data->error = 0;
    
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
        case TOKEN_DOLLAR: type_str = "DOLLAR"; break;
        case TOKEN_ENV_VAR: type_str = "ENV_VAR"; break;
        case TOKEN_CMD: type_str = "CMD"; break;
        case TOKEN_ARGUMENT: type_str = "ARGUMENT"; break;
        case TOKEN_FILE: type_str = "FILE"; break;
        case TOKEN_ERROR: type_str = "ERROR"; break;
        default: type_str = "UNKNOWN"; break;
    }
    
    printf("%-12s", type_str);
}

void cleanup_tokens(t_main_data *data)
{
    t_token *current = data->tok->token_list;
    t_token *next;
    
    while (current)
    {
        next = current->next_token;
        if (current->word)
            free(current->word);
        free(current);
        current = next;
    }
    
    if (data->tok)
        free(data->tok);
}

int main()
{
    t_main_data data;
    t_token *current;
    int token_count = 0;
    
    printf("=== MINISHELL TOKENIZER TEST ===\n\n");
    
    if (init(&data) != 0)
    {
        printf("❌ Initialization failed.\n");
        return (1);
    }
    
    printf("Input: \"%s\"\n\n", data.tok->input);
    
    // Parse input into tokens
    if (parser(&data) != 0)
    {
        printf("❌ Parsing failed.\n");
        cleanup_tokens(&data);
        return (1);
    }
    
    // Check if we have any tokens
    if (!data.tok->token_list)
    {
        printf("❌ No tokens were created!\n");
        cleanup_tokens(&data);
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
        printf(" | \"%s\"\n", current->word ? current->word : "(null)");
        current = current->next_token;
    }
    
    printf("\nTotal tokens: %d\n", token_count);
    
    // Clean up memory
    cleanup_tokens(&data);
    
    return (0);
}
