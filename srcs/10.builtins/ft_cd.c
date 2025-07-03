/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:43:06 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/02 21:41:21 by yvieira-         ###   ########.fr       */
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

	if (args[1] == NULL || args[1][0] == '~')
	{
		path = get_env_value_from_ctx(ctx, "HOME");
		if (path == NULL || *path == '\0')
			return (print_error(ctx, "cd: HOME not set\n", 2, 2), 1);
	}
	else if (args[2] != NULL)
		return (print_error(ctx, "cd: too many arguments\n", 2, 2), 1);
	else if (args[1][0] == '-' && args[1][1] == '\0')
	{
		path = get_env_value_from_ctx(ctx, "OLDPWD");
		if (path == NULL || *path == '\0')
			return (print_error(ctx, "cd: OLDPWD not set\n", 2, 2), 1);
		ft_putendl_fd((char *)path, STDOUT_FILENO);
	}
	else
		path = args[1];
	if (chdir(path) != 0)
		return (print_error(ctx, safe_strjoin(ctx, "minishell: cd: ", \
		(char *)path), 2, 2), 1);
	update_pwd_vars(ctx);
	return (EXIT_SUCCESS);
}
