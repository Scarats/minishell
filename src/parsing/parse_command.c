#include "../../minishell.h"

t_cmd_node *parse_expression(t_tokenizer *tok);
t_cmd_node *parse_and_or(t_tokenizer *tok);
t_cmd_node *parse_pipe(t_tokenizer *tok);
t_cmd_node *parse_command(t_tokenizer *tok);

t_cmd_node* parse_command(t_tokenizer *tok)
{
    t_cmd_node *node;
    t_token *token;

    node = create_command_node();
    while((token = get_next_token(tok)))
    {
        if(token->type == TOKEN_EOF || 
            token->type == TOKEN_PIPE ||
            token->type == TOKEN_AND ||
            token->type == TOKEN_OR ||
            token->type == TOKEN_RPAREN)
        {
            if(token->value)
                tok->pos -= ft_strlen(token->value);
            free(token->value);
            free(token);
            break;
        }
        if(token->type == TOKEN_WORD)
            add_argument(node->command, token->value);
        else if(token->type == TOKEN_REDIRECT_OUT)
        {
            free(token->value);
            free(token);
            token = get_next_token(tok);
            if(token && token->type == TOKEN_WORD)
            {
                node->command->file_out = ft_strdup(token->value);
                node->command->append_out = 0;
            }
        }
        else if(token->type == TOKEN_APPEND)
        {
            free(token->value);
            free(token);
            token = get_next_token(tok);
            if(token && token->type == TOKEN_WORD)
            {
                node->command->file_out = ft_strdup(token->value);
                node->command->append_out = 1;
            }
        }
        else if(token->type == TOKEN_REDIRECT_IN)
        {
            free(token->value);
            free(token);
            token = get_next_token(tok);
            if (token && token->type == TOKEN_WORD)
                node->command->file_in = ft_strdup(token->value);
        }
        if (token)
        {
            free(token->value);
            free(token);
        }
    }
    return(node);
}

t_cmd_node* parse_pipe(t_tokenizer *tok)
{
    t_cmd_node *left = parse_command(tok);
    
    t_token *token = get_next_token(tok);
    if (token && token->type == TOKEN_PIPE)
    {
        free(token->value);
        free(token);
        t_cmd_node *right = parse_pipe(tok);
        return create_operator_node(NODE_PIPE, left, right);
    }
    if (token)
    {
        if (token->value)
            tok->pos -= ft_strlen(token->value);
        free(token->value);
        free(token);
    }
    return(left);
}

t_cmd_node* parse_and_or(t_tokenizer *tok)
{
    int looping;
    t_node_type operation;
    t_cmd_node *left;
    t_token *token;
    
    token = get_next_token(tok);
    looping = (token != NULL && (token->type == TOKEN_AND || token->type == TOKEN_OR));
    left = parse_pipe(tok);
    while (looping)
    {
        if (token->type == TOKEN_AND)
            operation = NODE_AND;
        else
            operation = NODE_OR;
        free(token->value);
        free(token);
        t_cmd_node *right = parse_pipe(tok);
        left = create_operator_node(operation, left, right);
        token = get_next_token(tok);
        looping = (token != NULL && (token->type == TOKEN_AND || token->type == TOKEN_OR));
        if(!looping && token)
        {
            if (token->value)
                tok->pos -= ft_strlen(token->value);
            free(token->value);
            free(token);
        }
    }
    return(left);
}

t_cmd_node *parse_expression(t_tokenizer *tok)
{
    t_cmd_node *left;
    t_token *token;
    
    token = get_next_token(tok);
    left = parse_and_or(tok);
    if (token)
    {
        if (token->value)
            tok->pos -= ft_strlen(token->value);
        free(token->value);
        free(token);
    }
    return(left);
}

t_cmd_node *parse_command_line(char *input)
{
    t_tokenizer *tok;
    t_cmd_node *tree;

    tok = init_tokenizer(input);
    tree = parse_expression(tok);
    free(tok->input);
    free(tok);
    return tree;
}