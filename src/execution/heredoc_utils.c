#include "../minishell.h"

/* int process_heredocs_in_node(t_node *node, t_main_data *data)
{
    t_root *root;

    if (!node) 
        return 0;
    root = data->root;
    t_redir *redir = node->redirection;
    while (redir) {
        if (redir->type == TOKEN_HEREDOC) {
            int fd = heredoc(redir, data);
            printf("dup heredoc\n");  // Read heredoc into temp file
            if (fd < 0) {
                if (root->last_exit_status == 130) return 130;  // Propagate interrupt
                return 1;  // Other error
            }
            close(fd);  // Close after reading; redirections will reopen
            // Convert to input redirection for simplicity
            redir->type = TOKEN_REDIRECT_IN;
            // redir->filename is already set to temp file in heredoc()
        }
        redir = redir->next;
    }
    // Recurse for pipes/logical operators
    if (node->left) if (process_heredocs_in_node(node->left, data)) return 1;
    if (node->right) if (process_heredocs_in_node(node->right, data)) return 1;
    return 0;
}

// Wrapper function
int process_heredocs(t_main_data *data)
{
    if (!data || !data->node) return 0;
    return process_heredocs_in_node(data->node, data);
} */
