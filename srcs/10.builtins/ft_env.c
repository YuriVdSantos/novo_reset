/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 19:50:02 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/15 19:50:16 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_ctx *ctx)
{
	int		i;

	if (!ctx->env_list_str)
		return (EXIT_FAILURE);
	i = 0;
	while (ctx->env_list_str[i])
	{
		ft_putstr_fd(ctx->env_list_str[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
	ctx->exit_status = 0;
	return (EXIT_SUCCESS);
}
