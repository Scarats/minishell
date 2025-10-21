#include "../minishell.h"

char	*my_getcwd(t_root *root)
{
	char	*path;

	path = my_malloc(&root->list_of_list, &root->malloc_root, PATH_MAX + 1);
	if (!path)
		return (NULL);
	if (getcwd(path, PATH_MAX + 1) == NULL)
		return (NULL);
	return (path);
}

int	pwd(void)
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