/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:43:06 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/27 15:43:07 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror((char *)path);
		return (EXIT_FAILURE);
	}
	update_pwd_vars(ctx);
	return (EXIT_SUCCESS);
}
