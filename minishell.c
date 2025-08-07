#include "minishell.h"

/* int main()
{
	t_main_data data;
	traverse_tree(data.node, &data);
	return(0);
} */

//vibecoded testing programs

void print_tree(t_cmd_node *node, int depth) {
    if (!node) return;
    
    for (int i = 0; i < depth; i++) ft_printf("  ");
    
    switch (node->type) {
        case NODE_COMMAND:
            ft_printf("CMD: ");
            t_arg_node *current = node->command->args_first;
            while (current != NULL) {
                ft_printf("'%s' ", current->value);
                current = current->next;
            }
            if (node->command->file_in) ft_printf("< %s ", node->command->file_in);
            if (node->command->file_out) {
                ft_printf("%s %s", node->command->append_out? ">>" : ">", 
                       node->command->file_out);
            }
            ft_printf("\n");
            break;
        case NODE_PIPE:
            ft_printf("PIPE\n");
            print_tree(node->left, depth + 1);
            print_tree(node->right, depth + 1);
            break;
        case NODE_AND:
            ft_printf("AND (&&)\n");
            print_tree(node->left, depth + 1);
            print_tree(node->right, depth + 1);
            break;
        case NODE_OR:
            ft_printf("OR (||)\n");
            print_tree(node->left, depth + 1);
            print_tree(node->right, depth + 1);
            break;
    }
}

// Test the parser
int main()
{
    t_main_data data;

    char input[1024];
    
    printf("Shell Parser with Binary Tree\n");
    printf("Supports: pipes (|), logical operators (&&, ||), sequences (;)\n");
    printf("Enter commands to parse (or 'quit' to exit):\n");
    
    while (1) {
        printf("$ ");
        if (!fgets(input, sizeof(input), stdin)) break;
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "quit") == 0) break;
        if (strlen(input) == 0) continue;

        data.input = input;
        data.node = parse_command_line(&data);
        if (tree) {
            printf("Parse tree:\n");
            print_tree(tree, 0);
            printf("\n");
        }
        
        // TODO: Add proper cleanup function (lmao thanks claude)
    }
    
    return 0;
}