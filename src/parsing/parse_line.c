#include "./minishell.h"

tokenizer_t *tokenizer_initializer(char *input)
{
    tokenizer_t *tok = malloc(sizeof(tokenizer_t));
    tok->input = ft_strdup(input);
    tok->pos = 0;
    tok->length = ft_strlen(input);
    return(tok);
}

token_t *checktoken(tokenizer_t *tok)
{
    if(!tokenizer_initializer(tok))
        return(0);
}


void skipspace(tokenizer_t *tok)
{
    while (tok->pos < tok->length && ft_isspace(tok->input[tok->pos]))
        tok->pos++;
}

token_t *token_and_or(tokenizer_t *tok)
{
    char c;
    
    checktoken(tok);
    token_t *token = malloc(sizeof(token_t));
    c = tok->input[tok->pos];
    if(c == '&' && tok->pos +1 < tok->length && tok->input[tok->pos + 1] == '&')
    {
        token->type = TOKEN_AND;
        token->value = ft_strdup("&&");
        tok->pos += 2;
        return(token);
    }
    if(c == '|' && tok->pos +1 < tok->length && tok->input[tok->pos + 1] == '|')
    {
        token->type = TOKEN_OR;
        token->value = ft_strdup("||");
        tok->pos += 2;
        return(token);
    }
}

token_t *token_append_heredoc(tokenizer_t *tok)
{
    char c;
    
    checktoken(tok);
    token_t *token = malloc(sizeof(token_t));
    c = tok->input[tok->pos];
    if(c == '<' && tok->pos +1 < tok->length && tok->input[tok->pos + 1] == '<')
    {
        token->type = TOKEN_HEREDOC;
        token->value = ft_strdup("<<");
        tok->pos += 2;
        return(token);
    }
    if(c == '>' && tok->pos +1 < tok->length && tok->input[tok->pos + 1] == '>')
    {
        token->type = TOKEN_APPEND;
        token->value = ft_strdup(">>");
        tok->pos += 2;
        return(token);
    }
}

token_t *token_pipe(tokenizer_t *tok)
{
    char c;
    
    checktoken(tok);
    token_t *token = malloc(sizeof(token_t));
    c = tok->input[tok->pos];
    if(c == '|')
    {
        token->type = TOKEN_PIPE;
        token->value = ft_strdup("|");
        tok->pos += 1;
        return(token);
    }
}

token_t *token_redirect_io(tokenizer_t *tok)
{
    char c;
    
    checktoken(tok);
    token_t *token = malloc(sizeof(token_t));
    c = tok->input[tok->pos];
    if(c == '<')
    {
        token->type = TOKEN_REDIRECT_IN;
        token->value = ft_strdup("<");
        tok->pos += 1;
        return(token);
    }
    if(c == '>')
    {
        token->type = TOKEN_REDIRECT_OUT;
        token->value = ft_strdup(">");
        tok->pos += 1;
        return(token);
    }
}

