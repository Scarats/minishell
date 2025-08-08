#include "minishell.h"

int init(t_main_data *data)
{
	ft_memset(data, 0, sizeof(data));
	data->input = "This is a test";
	data->tok->length = ft_strlen(data->tok->input);
}

int main()
{
	t_main_data data;

	init(&data);
	traverse_tree(data.node, &data);
	return(0);
}
