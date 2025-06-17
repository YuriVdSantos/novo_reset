/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 23:30:24 by yvieira-          #+#    #+#             */
/*   Updated: 2025/06/17 14:27:16 by yurivieirad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	update_env_vars(t_env *minienv)
// {
//     char	cwd[PATH_MAX];
//     char	*pwd;

//     pwd = minienv_value("PWD", minienv);
//     if (pwd && *pwd)
//     {
//         minienv_update("OLDPWD", pwd, minienv);
//         minienv_update("PWD", getcwd(cwd, PATH_MAX), minienv);
//     }
// }

/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* ft_cd.c                                            :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/05/26 23:30:24 by yvieira-          #+#    #+#             */
/* Updated: 2025/06/17 14:15:00 by gemini           ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static void	update_pwd_vars(t_ctx *ctx)
{
	char	cwd[PATH_MAX];
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
