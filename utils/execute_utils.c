/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 20:38:48 by yvieira-          #+#    #+#             */
/*   Updated: 2025/07/02 20:45:43 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool validate_args(char **args, t_ctx *ctx)
{
	if (!args || !args[0])
	{
		free_context(ctx);
		exit(0);
	}
	return (true);
}

static char *resolve_command_path(char *cmd_name, t_ctx *ctx)
{
	char *path;

	if (ft_strchr(cmd_name, '/'))
		return (cmd_name);
		
	path = get_path(cmd_name, ctx);
	if (!path)
		exit_with_error(ctx, cmd_name, "command not found", 127);
	return (path);
}

static bool validate_command(char *path, char *cmd_name, t_ctx *ctx)
{
	struct stat path_stat;

	if (access(path, F_OK) == -1)
		exit_with_error(ctx, cmd_name, "No such file or directory", 127);
		
	if (stat(path, &path_stat) == -1)
		exit_with_error(ctx, cmd_name, "stat failed", 127);
		
	if (S_ISDIR(path_stat.st_mode))
		exit_with_error(ctx, cmd_name, "Is a directory", 126);
		
	if (access(path, X_OK) == -1)
		exit_with_error(ctx, cmd_name, "Permission denied", 126);
		
	return (true);
}

static void handle_exec_error(char *path, char *cmd_name, t_ctx *ctx)
{
	if (path != cmd_name)
		free(path);
	exit_with_error(ctx, cmd_name, strerror(errno), 126);
}

int execute_external(char **args, t_env *minienv, t_ctx *ctx)
{
	char	*path;
	char	**envp;

	if (!validate_args(args, ctx))
		return (1);
	path = resolve_command_path(args[0], ctx);
	if (!path)
		return (1);
	if (!validate_command(path, args[0], ctx))
		return (1);
	envp = minienv_to_envp(minienv);
	execve(path, args, envp);
	handle_exec_error(path, args[0], ctx);
	return (1);
}