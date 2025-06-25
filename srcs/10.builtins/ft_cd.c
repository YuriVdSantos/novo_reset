#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static void	update_pwd_vars(t_ctx *ctx)
{
	char	cwd[4096];
	t_env	*pwd_var;
	char	*assignment;

	pwd_var = find_env_var(ctx->env_list, "PWD");
	if (pwd_var && pwd_var->value)
	{
		assignment = ft_strjoin("OLDPWD=", pwd_var->value);
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

static char	*get_env_value_from_ctx(t_ctx *ctx, const char *key)
{
	t_env	*var;

	var = find_env_var(ctx->env_list, key);
	if (var)
		return (var->value);
	return (NULL);
}

int	ft_cd(char **args, t_ctx *ctx)
{
	const char	*path;
	char		*path_to_go;

	path_to_go = NULL;
	if (args[1] == NULL)
	{
		path = get_env_value_from_ctx(ctx, "HOME");
		if (path == NULL || *path == '\0')
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
	}
	else if (args[2] != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else if (args[1][0] == '-' && args[1][1] == '\0')
	{
		path = get_env_value_from_ctx(ctx, "OLDPWD");
		if (path == NULL || *path == '\0')
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		ft_putendl_fd((char *)path, STDOUT_FILENO);
	}
	else if (args[1][0] == '~')
	{
		char *home = get_env_value_from_ctx(ctx, "HOME");
		if (home == NULL || *home == '\0')
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		path_to_go = ft_strjoin(home, args[1] + 1);
		path = path_to_go;
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror((char *)path);
		if (path_to_go)
			free(path_to_go);
		return (EXIT_FAILURE);
	}
	update_pwd_vars(ctx);
	if (path_to_go)
		free(path_to_go);
	return (EXIT_SUCCESS);
}