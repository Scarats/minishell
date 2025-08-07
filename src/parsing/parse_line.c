#include "../../minishell.h"

t_tokenizer *tokenizer_initializer(char *input)
{
    t_tokenizer *tok = malloc(sizeof(t_tokenizer));
    tok->input = ft_strdup(input);
    tok->pos = 0;
    tok->length = ft_strlen(input);
    return(tok);
}

t_token *checktoken(t_tokenizer *tok)
{
    if (!tok || !tok->input)
        return(0);
    return(t_token *)1;
}


void ft_skipspace(t_tokenizer *tok)
{
    int isspace;
    
    isspace = ft_isspace(&tok->input[tok->pos]);
    while (tok->pos < tok->length && isspace)
        tok->pos++;
}

t_token *token_and_or(t_tokenizer *tok)
{
    char c;
    
    if (!checktoken(tok))
        return(0);
    t_token *token = malloc(sizeof(t_token));
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
    return(0);
}

t_token *token_append_heredoc(t_tokenizer *tok)
{
    char c;
    
    if (!checktoken(tok))
        return(0);
    t_token *token = malloc(sizeof(t_token));
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
    return(0);
}

t_token *token_pipe(t_tokenizer *tok)
{
    char c;
    
    if (!checktoken(tok))
        return(0);
    t_token *token = malloc(sizeof(t_token));
    c = tok->input[tok->pos];
    if(c == '|')
    {
        token->type = TOKEN_PIPE;
        token->value = ft_strdup("|");
        tok->pos += 1;
        return(token);
    }
    return(0);
}

t_token *token_redirect_io(t_tokenizer *tok)
{
    char c;
    
    if (!checktoken(tok))
        return(0);
    t_token *token = malloc(sizeof(t_token));
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
    return(0);
}

t_token *token_quote(t_tokenizer *tok)
{
    char c;
    char q;
    int start;
    int len;
    
    if (!checktoken(tok))
        return(0);
    t_token *token = malloc(sizeof(t_token));
    c = tok->input[tok->pos];
    if (c == '"')
    {
        q = c;
        tok->pos++;
        start = tok->pos;
        while (tok->pos < tok->length && tok->input[tok->pos] != q)
            tok->pos++;
        if (tok->pos >= tok->length)
        {
            ft_printf(stderr, "Error: Unmatched quote\n");
            free(token);
            return(0);
        }
        len = tok->pos - start;
        token->type = TOKEN_WORD;
        token->value = malloc(len + 1);
        ft_strncpy(token->value, &tok->input[start], len);
        token->value[len] = '\0';
        tok->pos++;
        return(token);
    }
    free(token);
    return(0);
}

t_token *token_word(t_tokenizer *tok)
{
    int start;
    int len;
    t_token *token;
    
    if (!checktoken(tok))
        return(0);
    start = tok->pos;
    while (tok->pos < tok->length && 
           !ft_isspace(tok->input[tok->pos]) &&
           tok->input[tok->pos] != '|' &&
           tok->input[tok->pos] != '<' &&
           tok->input[tok->pos] != '>' &&
           tok->input[tok->pos] != '&' &&
           tok->input[tok->pos] != '"')
        tok->pos++;
    len = tok->pos - start;
    if (len == 0)
        return(0);
    token = malloc(sizeof(t_token));
    token->type = TOKEN_WORD;
    token->value = malloc(len + 1);
    ft_strncpy(token->value, &tok->input[start], len);
    token->value[len] = '\0';
    return(token);
}

t_token *get_next_token(t_tokenizer *tok)
{
    t_token *token;
    
    if (!tok || tok->pos >= tok->length)
        return(NULL);
    ft_skipspace(tok);
    if (tok->pos >= tok->length)
        return(NULL);
    token = token_and_or(tok);
    if (token)
        return(token);
    token = token_append_heredoc(tok);
    if (token)
        return(token);
    token = token_pipe(tok);
    if (token)
        return(token);
    token = token_redirect_io(tok);
    if (token)
        return(token);
    token = token_quote(tok);
    if (token)
        return(token);
    token = token_word(tok);
    if (token)
        return(token);
    tok->pos++;
    return(get_next_token(tok));
}
