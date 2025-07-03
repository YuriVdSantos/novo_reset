/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 20:38:48 by yvieira-          #+#    #+#             */
/*   Updated: 2025/07/02 20:48:19 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_with_error(t_ctx *ctx, const char *arg, \
	const char *msg, int code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd((char *)msg, STDERR_FILENO);
	if (msg[ft_strlen(msg) - 1] != '\n')
		ft_putstr_fd("\n", STDERR_FILENO);
	free_context(ctx);
	exit(code);
}
