/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignment_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 01:16:01 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/26 01:16:17 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_single_assignment(t_ctx *ctx, char *arg)
{
	char	*equal_pos;

	equal_pos = safe_strchr(ctx, arg, '=');
	if (equal_pos && (equal_pos != arg))
		set_env_var(ctx, arg);
}

void	process_assignments(t_ctx *ctx, t_cmd *cmd)
{
	int		i;

	i = 0;
	if (!cmd || !cmd->args)
		return ;
	while (cmd->args[i])
	{
		process_single_assignment(ctx, cmd->args[i]);
		i++;
	}
}
