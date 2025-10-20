#include "../minishell.h"

int	cd(t_root *root, t_node *node)
{
	int		error;
	char	*pwd;
	char	*old_pwd;

	error = 0;
	if (arraylen(node->cmd_argv) > 2)
		return (fdprintf(2, "error: too many arguments.\n"), 1);
	if (error)
		return (error);
	if (node->cmd_argv[1] == NULL)
		node->cmd_argv[1] = get_env_var(root->env, "HOME");
	old_pwd = ft_strjoin("OLDPWD=", my_getcwd(root));
	my_addtolist(&root->malloc_root, old_pwd);
	export(root, (char *[]){"export", old_pwd, NULL});
	error = chdir(node->cmd_argv[1]);
	if (error)
		return (fdprintf(2, "minishell: cd: %s: %s\n", node->cmd_argv[1],
				strerror(errno)), 1);
	pwd = ft_strjoin("PWD=", my_getcwd(root));
	my_addtolist(&root->malloc_root, pwd);
	export(root, (char *[]){"export", pwd, NULL});
	return (error);
}
