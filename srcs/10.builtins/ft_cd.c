/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 23:30:24 by yvieira-          #+#    #+#             */
/*   Updated: 2025/05/27 03:05:58 by yvieira-         ###   ########.fr       */
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

int	ft_cd(char **args, t_ctx *ctx)
{
    const char	*path;

    if (args[1] && args[2])
        return (cd_error());
	else if (args[1])
		path = args[1];
	else
		path = get_env_value(ctx, "HOME");
	if (chdir(path) != 0)
    {
        print_error_msg("cd", args[1]);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int	cd_error(void)
{
    print_error_msg("cd", "too many arguments");
    return (EXIT_FAILURE);
}