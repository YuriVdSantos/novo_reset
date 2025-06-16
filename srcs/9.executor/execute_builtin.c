/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:02:38 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/13 13:36:44 by yurivieirad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	execute_forked_builtin(char **args, t_ctx *ctx)
{
	int	exit_status;

	exit_status = execute_builtin(args, ctx);
	exit(exit_status);
}

int	execute_builtin(char **args, t_ctx *ctx)
{
	char	*command;

	command = args[0];
	if (str_equal(command, "echo"))
		return (ft_echo(args));
	if (str_equal(command, "pwd"))
		return (ft_pwd());
	if (str_equal(command, "env"))
		return (ft_env(ctx));
	if (str_equal(command, "export"))
		return (ft_export(args, ctx));
	if (str_equal(command, "unset"))
		return (ft_unset(args, ctx));
	if (str_equal(command, "cd"))
		return (ft_cd(args, ctx));
	if (str_equal(command, "exit"))
		return (ft_exit(args, ctx));
	else
		return (EXIT_FAILURE);
}
