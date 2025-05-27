/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dispatcher.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 05:38:14 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/27 02:04:46 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin(t_ctx *ctx, t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
	{
		handle_error(ctx, "No command provided", 1, 1);
		return ;
	}
	if (!is_builtin(cmd->args[0]))
	{
		handle_error(ctx, "Command not found", 127, 127);
		return ;
	}
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		builtin_cd(ctx, cmd);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		builtin_echo(ctx, cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(ctx, cmd);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		builtin_export(ctx, cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		builtin_pwd(ctx, cmd);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		builtin_unset(ctx, cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		builtin_env(ctx, cmd);
}
