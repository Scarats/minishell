#include "../minishell.h"

// https://www.youtube.com/watch?v=j9yL30R6npk

// Go to ".." RECURSIVELY while not at root / home.
int cmd_pwd(DIR *dir)
{
	struct dirent *metadata;

	if (!dir)
		return (1);
	metadata = readdir(dir);	
	if (ft_strncmp(metadata->d_name, "/", sizeof(metadata->d_name)) != 0)
	{
		dir = opendir("..");
		cmd_pwd(dir);
	}
	printf("/%s", metadata->d_name);
	return (0);
}