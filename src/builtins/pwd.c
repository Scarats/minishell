#include "../minishell.h"

int pwd(void)
{
	char *path;

	path = malloc(sizeof(PATH_MAX) + 1);
	if (!path)
		return (1);
	getcwd(path, PATH_MAX + 1);
	if (!path)
		return (1);
	printf("%s\n", path);
	free(path);
	return (0);
}