/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 21:58:34 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/01 14:34:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_msg(char *command, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (command)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(msg, STDERR_FILENO);
}

void	print_error(t_ctx *ctx, char *context_msg, int errnum, int exit_status)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (context_msg)
	{
		ft_putstr_fd(context_msg, STDERR_FILENO);
	}
	if (errnum > 0)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errnum), STDERR_FILENO);
	}
	ft_putstr_fd("\n", STDERR_FILENO);
	if (ctx)
		ctx->exit_status = exit_status;
	else
		g_signal = exit_status;
}

bool	handle_error(t_ctx *ctx, char *msg, int errnum, int exit_status)
{
	print_error(ctx, msg, errnum, exit_status);
	return (false);
}

bool	syntax_error(t_ctx *ctx, char *msg)
{
	char	*error_msg;
	char	*tmp;

	tmp = safe_strjoin(ctx, "syntax error near unexpected token `", msg);
	error_msg = safe_strjoin(ctx, tmp, "'");
	print_error(ctx, error_msg, 0, 2);
	return (false);
}
