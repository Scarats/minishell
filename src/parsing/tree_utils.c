#include "../minishell.h"

// take token type and return node type.
t_node_type map_token_to_node(t_token_type t)
{
    if (t == TOKEN_PIPE)
        return (NODE_PIPE);
    if (t == TOKEN_AND_AND)
        return (NODE_AND);
    if (t == TOKEN_OR)
        return (NODE_OR);
    return (NODE_COMMAND);
}

int add_redirection(t_token_type)
{
	// Check if it's the first redirection of the list.
	// Add the new redirection to the list.

}