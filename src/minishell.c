#include "minishell.h"
#include <errno.h>

int init(t_main_data *data)
{
    data->tok = malloc(sizeof(t_tokenizer));
    if (!data->tok)
        return (1);
    ft_memset(data->tok, 0, sizeof(t_tokenizer));
    data->tok->prev_char_type = CHAR_SPACE;
    data->tok->curr_char_type = CHAR_NULL;
    data->tok->double_quote = false;
    data->tok->single_quote = false;
    data->node = NULL;
    data->malloc_tok = NULL;
    data->malloc_tree = NULL;
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

static void reset_tokenizer_for_line(t_tokenizer *tok, char *line)
{
    tok->input = line;
    tok->length = (int)ft_strlen(line);
    tok->pos = 0;
    tok->prev_pos = 0;
    tok->prev_char_type = CHAR_SPACE;
    tok->curr_char_type = CHAR_NULL;
    tok->double_quote = false;
    tok->single_quote = false;
    tok->token_list = NULL;
    tok->last_token = NULL;
    tok->token_list_size = 0;
}

int main(void)
{
    t_main_data data;
    t_token *current;
    int token_count;
    char *line = NULL;
    size_t cap = 0;
    ssize_t nread;

    if (init(&data) != 0)
    {
        fprintf(stderr, "init failed\n");
        return 1;
    }

    while (1)
    {
        printf("minishell> ");
        fflush(stdout);

        nread = getline(&line, &cap, stdin);
        if (nread == -1)
        {
            if (feof(stdin))
                printf("exit\n");
            else
                perror("getline");
            break;
        }
        if (nread > 0 && line[nread - 1] == '\n')
            line[nread - 1] = '\0';
        if (line[0] == '\0')
            continue;
        if (!ft_strncmp(line, "exit", 5))
            break;

        reset_tokenizer_for_line(data.tok, line);

        if (parser(&data) != 0)
        {
            fprintf(stderr, "parse error\n");
            my_free(&data.malloc_tok);
            data.malloc_tok = NULL;
            continue;
        }

        printf("\nNo.  | %-12s | Value\n", "Type");
        printf("----------------------------------\n");
        current = data.tok->token_list;
        token_count = 0;
        while (current)
        {
            printf("%-4d | ", ++token_count);
            print_token_type(current->type);
            printf(" | \"%s\"\n", current->word ? current->word : "(null)");
            current = current->next_token;
        }
        printf("\nTotal tokens: %d\n\n", token_count);

        // Free tokens + words allocated via my_malloc
        my_free(&data.malloc_tok);
        data.malloc_tok = NULL;
    }

    free(line);
    free(data.tok);
    return 0;
}
