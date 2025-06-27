/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:23:13 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/27 15:23:14 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(char **args, t_ctx *ctx)
{
	if (!args || !args[0])
		return (0);
	if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(args, ctx));
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args, ctx));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(args[0], "export") == 0)
		return (ft_export(args, ctx));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unset(args, ctx));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(ctx));
	if (ft_strcmp(args[0], "exit") == 0)
		return (ft_exit(args, ctx));
	return (127);
}
