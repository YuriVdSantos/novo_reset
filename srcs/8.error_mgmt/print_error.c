/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 16:09:46 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/26 01:29:21 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// talvez nem precise
char	*get_error_message(int error_code)
{
	if (error_code == 0)
		return (ERR_SUCCESS_MSG);
	else if (error_code == 1)
		return (ERR_GENERAL_MSG);
	else if (error_code == 2)
		return (ERR_MISUSE_SHELL_MSG);
	else if (error_code == 126)
		return (ERR_CANT_EXECUTE_MSG);
	else if (error_code == 127)
		return (ERR_CMD_NOT_FOUND_MSG);
	else if (error_code == 128)
		return (ERR_EXIT_ARG_MSG);
	else if (error_code == 130)
		return (ERR_CTRL_C_MSG);
	else if (error_code == 137)
		return (ERR_KILL_9_MSG);
	else if (error_code == 139)
		return (ERR_SEGFAULT_MSG);
	else if (error_code == 255)
		return (ERR_EXIT_RANGE_MSG);
	else
		return (ERR_UNKNOWN_MSG);
}

bool	handle_error(t_ctx *ctx, char *msg, int errnum, int exit_status)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (errnum != -1)
	{
		if (errnum == 0)
			perror(msg);
		else
			ft_putendl_fd(strerror(errnum), STDERR_FILENO);
	}
	else
		ft_putendl_fd(msg, STDERR_FILENO);
	if (ctx && exit_status != -1)
		ctx->exit_status = exit_status;
	return (false);
}

// Versão para erros de sintaxe
bool	syntax_error(t_ctx *ctx, char *msg)
{
	return (handle_error(ctx, msg, -1, 2));
}

// Versão para erros gerais
void	print_error(t_ctx *ctx, char *msg, int errnum, int exit_status)
{
	handle_error(ctx, msg, errnum, exit_status);
}
