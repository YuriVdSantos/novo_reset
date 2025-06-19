

#include "minishell.h"

static void	update_pwd_vars(t_ctx *ctx)
{
	char	cwd[4096];
	char	*old_pwd_value;
	char	*assignment;

	old_pwd_value = get_env_value(ctx, "PWD");
	if (old_pwd_value)
	{
		assignment = ft_strjoin("OLDPWD=", old_pwd_value);
		if (assignment)
		{
			set_env_var(ctx, assignment);
			free(assignment);
		}
	}

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		assignment = ft_strjoin("PWD=", cwd);
		if (assignment)
		{
			set_env_var(ctx, assignment);
			free(assignment);
		}
	}
	else
		perror("minishell: cd: error retrieving current directory");
}

int	ft_cd(char **args, t_ctx *ctx)
{
	const char	*path;

	if (args[1] == NULL)
	{
		path = get_env_value(ctx, "HOME");
		if (path == NULL || *path == '\0')
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
	}
	else if(args[2] != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else if (args[1][0] == '-' && args[1][1] == '\0')
	{
		path = get_env_value(ctx, "OLDPWD");
		if (path == NULL || *path == '\0')
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(path);
		return (EXIT_FAILURE);
	}
	update_pwd_vars(ctx);
	return (EXIT_SUCCESS);
}
