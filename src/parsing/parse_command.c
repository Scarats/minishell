#include "./minishell.h"

t_cmd_node* parse_expression(t_tokenizer *tok);
t_cmd_node* parse_and_or(t_tokenizer *tok);
t_cmd_node* parse_pipeline(t_tokenizer *tok);
t_cmd_node* parse_command(t_tokenizer *tok);

t_cmd_node* parse_command(t_tokenizer *tok)
{
    t_cmd_node *node = create_command_node();
    t_token *token;
    
    while ((token = get_next_token(tok)) != NULL) {
        if (token->type == TOKEN_EOF || 
            token->type == TOKEN_PIPE ||
            token->type == TOKEN_AND ||
            token->type == TOKEN_OR ||
            token->type == TOKEN_RPAREN) {
            tok->pos -= strlen(token->value ? token->value : "");
            free(token->value);
            free(token);
            break;
        }
        
        if (token->type == TOKEN_WORD) {
            add_argument(node->command, token->value);
        }
        else if (token->type == TOKEN_REDIRECT_OUT) {
            free(token->value);
            free(token);
            token = get_next_token(tok);
            if (token && token->type == TOKEN_WORD) {
                node->command->file_out = ft_strdup(token->value);
                node->command->append_out = 0;
            }
        }
        else if (token->type == TOKEN_APPEND) {
            free(token->value);
            free(token);
            token = get_next_token(tok);
            if (token && token->type == TOKEN_WORD)
            {
                node->command->file_out = strdup(token->value);
                node->command->append_out = 1;
            }
        }
        else if (token->type == TOKEN_REDIRECT_IN)
        {
            free(token->value);
            free(token);
            token = get_next_token(tok);
            if (token && token->type == TOKEN_WORD){
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
}

t_cmd_node* parse_pipeline(tokenizer_t *tok) {
    cmd_node_t *left = parse_command(tok);
    
    token_t *token = get_next_token(tok);
    if (token && token->type == TOKEN_PIPE) {
        free(token->value);
        free(token);
        cmd_node_t *right = parse_pipeline(tok);  // Right associative
        return create_operator_node(NODE_PIPE, left, right);
    }
    
    // Put token back
    if (token) {
        tok->pos -= strlen(token->value ? token->value : "");
        free(token->value);
        free(token);
    }
    
    return left;
}

// Parse && and || (same precedence, left associative)
t_cmd_node* parse_and_or(tokenizer_t *tok) {
    t_cmd_node *left = parse_pipeline(tok);
    
    while (1) {
        token_t *token = get_next_token(tok);
        if (!token || (token->type != TOKEN_AND && token->type != TOKEN_OR)) {
            if (token) {
                tok->pos -= strlen(token->value ? token->value : "");
                free(token->value);
                free(token);
            }
            break;
        }
        
        node_type_t op_type = (token->type == TOKEN_AND) ? NODE_AND : NODE_OR;
        free(token->value);
        free(token);
        
        t_cmd_node *right = parse_pipeline(tok);
        left = create_operator_node(op_type, left, right);
    }
    
    return left;
}

// Parse full expression (lowest precedence: semicolon)
t_cmd_node *parse_expression(tokenizer_t *tok)
{
    t_cmd_node *left = parse_and_or(tok);
    
    token_t *token = get_next_token(tok);
    if (token && token->type == TOKEN_SEMICOLON) {
        free(token->value);
        free(token);
        t_cmd_node *right = parse_expression(tok);  // Right associative
        return create_operator_node(NODE_SEQUENCE, left, right);
    }
    
    // Put token back
    if (token) {
        tok->pos -= strlen(token->value ? token->value : "");
        free(token->value);
        free(token);
    }
    
    return left;
}
t_cmd_node *parse_command_line(char *input)
{
    tokenizer_t *tok = init_tokenizer(input);
    t_cmd_node *tree = parse_expression(tok);
    
    free(tok->input);
    free(tok);
    return tree;
}