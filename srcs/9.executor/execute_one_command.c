/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:30:54 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/02 20:32:49 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute(t_ctx *ctx, t_cmd *command, int *original_fds);

int	execute_one_command(t_cmd *command, t_ctx *ctx)
{
	int		original_fds[2];

	if (!command || !command->args || !command->args[0])
		return (g_signal);
	if (apply_redirections(command, original_fds) == FAILED)
	{
		restore_original_fds(original_fds);
		return (1);
	}
	if (is_builtin(command->args[0]))
	{
		g_signal = execute_builtin(command->args, ctx);
	}
	else
	{
		if (!(execute(ctx, command, original_fds)))
			return (1);
	}
	restore_original_fds(original_fds);
	return (g_signal);
}

static int	execute(t_ctx *ctx, t_cmd *command, int *original_fds)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		print_error(ctx, "fork", -1, 1);
		restore_original_fds(original_fds);
		return (0);
	}
	if (pid == 0)
	{
		define_non_interactive_signals();
		execute_external(command->args, ctx->env_list, ctx);
		return (1);
	}
	else
	{
		define_execute_signals(pid);
		g_signal = wait_for_child(pid, 1, ctx);
		define_interactive_signals();
		return (1);
	}
	return (1);
}
