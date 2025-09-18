#include "../minishell.h"

int pwd(void)
{
	char *path;

	path = malloc(PATH_MAX + 1);
	if (!path)
		return (1);
	if (getcwd(path, PATH_MAX + 1) == NULL)
		return (free(path), fdprintf(2, "pwd: %s\n", strerror(errno), 1));

	printf("%s\n", path);
	free(path);
	return (0);
}