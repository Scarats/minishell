#include "minishell.h"

int main()
{
	t_main_data data;
	traverse_tree(data.node, &data);
	return(0);
}