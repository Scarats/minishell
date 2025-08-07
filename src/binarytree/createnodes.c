#include "../../minishell.h"

t_cmd_node *command_node_init(void)
{
    t_cmd_node *node = malloc(sizeof(t_cmd_node));
    if(!node)
        return(0);
    node->type = NODE_COMMAND;
    node->command = malloc(sizeof(t_command));
    node->command->args = malloc(sizeof(char*) * 32);
    node->command->argc = 0;
    node->command->file_in = NULL;
    node->command->file_out = NULL;
    node->command->append_out = 0;
    node->command->exit_status = 0;
    node->exit_status = 0;
    return(node);
}

t_cmd_node *operator_node_init(t_node_type type, t_cmd_node *left, t_cmd_node *right)
{
    t_cmd_node *node = malloc(sizeof(t_cmd_node));
    if(!node)
        return(0);
    node->type = type;
    node->left = left;
    node->right = right;
    node->exit_status = 0;
    return(node);
}

void *argument(t_command *cmd, char *args)
{
    t_arg_node *new_node; 
    new_node = malloc(sizeof(t_arg_node));
    new_node->value = ft_strdup(args);
    new_node->next = NULL;
    
    if (cmd->args_first == NULL)
    {
        cmd->args_first = new_node;
        cmd->args_last = new_node;
    }
    else
    {
        cmd->args_last->next = new_node;
        cmd->args_last = new_node;
    }
    
    cmd->argc++;
}

char **args_to_array(t_command *cmd)
{
    int i;
    char **args;
    t_arg_node *current;

    i = 0;
    args = malloc(sizeof(char*) * (cmd->argc + 1));
    current = cmd->args_first;
    while (current)
    {
        args[i] = current->value;
        current = current->next;
        i++;
    }
    args[i] = 0;
    return(args);
}