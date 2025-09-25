#include "minishell.h"
#include <errno.h>

static void noop_free(void *p) { (void)p; }

volatile sig_atomic_t stop_flag = 0;

int init(t_root *root)
{
    t_main_data *data;

    data = root->data;
    data->malloc_tok = NULL;
    data->malloc_tree = NULL;

    // DO NOT reset the persistent pools here; they were set in main
    // root->list_of_list = NULL;
    // root->malloc_root = NULL;

    // Allocate tokenizer in the persistent pool (root), not per-line
    data->tok = my_malloc(&root->list_of_list, &root->malloc_root, sizeof(t_tokenizer));
    if (!data->tok)
        return (1);
    ft_memset(data->tok, 0, sizeof(t_tokenizer));

    data->root = root;
    data->tok->prev_char_type = CHAR_SPACE;
    data->tok->curr_char_type = CHAR_NULL;
    data->tok->double_quote = false;
    data->tok->single_quote = false;
    data->node = NULL;

    data->error = 0;
    data->in_child = false;
    return (0);
}

// void print_token_type(t_token_type type)
// {
//     char *type_str;
//     switch (type)
//     {
//         case TOKEN_NULL: type_str = "NULL"; break;
//         case TOKEN_TEXT: type_str = "TEXT"; break;
//         case TOKEN_SPACE: type_str = "SPACE"; break;
//         case TOKEN_PIPE: type_str = "PIPE"; break;
//         case TOKEN_AND: type_str = "AND"; break;
//         case TOKEN_OR: type_str = "OR"; break;
//         case TOKEN_AND_AND: type_str = "AND_AND"; break;
//         case TOKEN_REDIRECT_IN: type_str = "REDIRECT_IN"; break;
//         case TOKEN_REDIRECT_OUT: type_str = "REDIRECT_OUT"; break;
//         case TOKEN_APPEND: type_str = "APPEND"; break;
//         case TOKEN_HEREDOC: type_str = "HEREDOC"; break;
//         case TOKEN_LPAREN: type_str = "LPAREN"; break;
//         case TOKEN_RPAREN: type_str = "RPAREN"; break;
//         case TOKEN_DOLLAR: type_str = "DOLLAR"; break;
//         case TOKEN_ENV_VAR: type_str = "ENV_VAR"; break;
//         case TOKEN_CMD: type_str = "CMD"; break;
//         case TOKEN_ARGUMENT: type_str = "ARGUMENT"; break;
//         case TOKEN_FILE: type_str = "FILE"; break;
//         case TOKEN_ERROR: type_str = "ERROR"; break;
//         default: type_str = "UNKNOWN"; break;
//     }
//     printf("%-12s", type_str);
// }

// // ---- AST printer helpers (pretty) ----
// static const char *node_type_str(t_node_type t)
// {
//     if (t == NODE_COMMAND) return "CMD";
//     if (t == NODE_PIPE)    return "PIPE";
//     if (t == NODE_AND)     return "AND";
//     if (t == NODE_OR)      return "OR";
//     return "UNKNOWN";
// }

// static const char *redir_type_str(t_token_type t)
// {
//     if (t == TOKEN_REDIRECT_IN)  return "<";
//     if (t == TOKEN_REDIRECT_OUT) return ">";
//     if (t == TOKEN_APPEND)       return ">>";
//     if (t == TOKEN_HEREDOC)      return "<<";
//     return "?";
// }

// static void print_cmd_inline(const t_node *node)
// {
//     int i = 0;
//     if (node->cmd_argv && node->cmd_argv[0])
//     {
//         while (node->cmd_argv[i])
//         {
//             if (i) printf(" ");
//             printf("%s", node->cmd_argv[i]);
//             i++;
//         }
//     }
//     else
//     {
//         printf("(empty)");
//     }
//     // Append redirs inline for quick glance
//     t_redir *r = node->redirection;
//     while (r)
//     {
//         printf(" [%s %s]", redir_type_str(r->type), r->filename ? r->filename : "(null)");
//         r = r->next;
//     }
// }

// static void print_ast_pretty_rec(t_node *node, const char *prefix, int is_last)
// {
//     if (!node) return;

//     // Branch characters
//     const char *branch = is_last ? "└── " : "├── ";
//     const char *pad    = is_last ? "    " : "│   ";

//     // Current line
//     printf("%s%s", prefix, branch);
//     if (node->type == NODE_COMMAND)
//     {
//         printf("CMD: ");
//         print_cmd_inline(node);
//         printf("\n");

//         // If you prefer redirs on their own lines (instead of inline),
//         // uncomment below and remove them from print_cmd_inline above.
//         /*
//         int count = 0;
//         for (t_redir *tmp = node->redirection; tmp; tmp = tmp->next) count++;
//         int idx = 0;
//         for (t_redir *r = node->redirection; r; r = r->next, idx++)
//         {
//             char next_prefix[1024];
//             snprintf(next_prefix, sizeof(next_prefix), "%s%s", prefix, pad);
//             int r_last = (idx == count - 1);
//             printf("%s%sredir %s %s\n",
//                    next_prefix,
//                    r_last ? "└── " : "├── ",
//                    redir_type_str(r->type),
//                    r->filename ? r->filename : "(null)");
//         }
//         */
//         return; // command has no tree children
//     }
//     else
//     {
//         printf("%s\n", node_type_str(node->type));
//     }

//     // Prepare child prefix
//     char next_prefix[1024];
//     snprintf(next_prefix, sizeof(next_prefix), "%s%s", prefix, pad);

//     // Collect real children (left/right)
//     t_node *children[2];
//     int n = 0;
//     if (node->left)  children[n++] = node->left;
//     if (node->right) children[n++] = node->right;

//     for (int i = 0; i < n; i++)
//     {
//         int child_is_last = (i == n - 1);
//         print_ast_pretty_rec(children[i], next_prefix, child_is_last);
//     }
// }

// static void print_ast(t_node *root)
// {
//     if (!root)
//     {
//         printf("(empty AST)\n");
//         return;
//     }
//     // Root line without a leading branch
//     printf("%s\n", node_type_str(root->type));
//     // Then its children (so the root looks like a top header)
//     t_node *children[2];
//     int n = 0;
//     if (root->type == NODE_COMMAND)
//     {
//         // For a single command root, print details as children-like lines
//         // to keep a consistent visual shape.
//         // Inline form (same as node line) for brevity:
//         printf("└── ");
//         print_cmd_inline(root);
//         printf("\n");
//         return;
//     }
//     if (root->left)  children[n++] = root->left;
//     if (root->right) children[n++] = root->right;

//     for (int i = 0; i < n; i++)
//     {
//         int child_is_last = (i == n - 1);
//         print_ast_pretty_rec(children[i], "", child_is_last);
//     }
// }
// // ---- end AST printer helpers ----

static void reset_tokenizer_for_line(t_tokenizer *tok, char *line)
{
    // Reinitialize tokenizer state for the new line
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

void handler(int sig)
{
	(void)sig;
    stop_flag = 1;
}

int main(int ac, char **av, char **envp)
{
    struct sigaction sa;
    // t_token *current;         // removed: no longer printing tokens
    // int token_count;          // removed: no longer printing tokens
    char *line = NULL;
    size_t cap = 0;
    ssize_t nread;
    t_root root;
    t_main_data data;

    ft_memset(&root, 0, sizeof(root));
    ft_memset(&data, 0, sizeof(data));

    (void)ac;   
	(void)av;   

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);

    // Initialize allocation root and env before using my_malloc/my_free
    root.malloc_root = NULL;
    root.env = NULL;
    root.list_of_list = NULL;  // <-- add this

    root.data = &data;
    root.data->root = &root;
    // set_env_var_list now takes t_list **; pass the address so it can update the head
    root.env = set_env_var_list(&root, envp);
	
    if (init(&root) != 0)
    {
        fprintf(stderr, "init failed\n");
        return 1;
    }
    while (1)
    {
        printf("minishell> ");
        fflush(stdout);
        // init(&data);  // REMOVE: this re-mallocs data->tok and leaks the previous one

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

        // Track the getline buffer in the per-line allocations
        my_addtolist(&data.malloc_tok, (void *)line);

        if (parser(&data) != 0)
        {
            fprintf(stderr, "parse error\n");
            // Free anything allocated for this line (tokens/tree/line) before continuing
            if (data.malloc_tok)
                my_free(&data.malloc_tok);
            line = NULL;   // prevent reuse by getline and double-free
            cap = 0;
            my_free(&data.malloc_tree);
            data.node = NULL;
            stop_flag = 0; // reset stop_flag after cleaning
            continue;
        }

        // Tokens and getline buffer are not needed anymore; free them now
        if (data.malloc_tok)
            my_free(&data.malloc_tok);
        line = NULL;   // getline will allocate a fresh buffer next time
        cap = 0;

        // Execute without debug prints
        root.last_exit_status = traverse_tree(data.node, &data);
		set_last_exit_status_var(&root);
        // Free allocations for this iteration (AST) tracked by my_malloc
        my_free(&data.malloc_tree);
        data.node = NULL;
        stop_flag = 0; // reset stop_flag after cleaning
    }

	my_multi_free(&root.list_of_list);
    // Free global resources
    my_free(&root.malloc_root);
    if (data.malloc_tok)
        my_free(&data.malloc_tok);
    my_free(&data.malloc_tree);
    free(line);    // safe: NULL if already freed via my_free

    // Also free the tracking list nodes themselves (optional, silences "still reachable")
    ft_lstclear(&root.list_of_list, noop_free);

    return 0;
}
