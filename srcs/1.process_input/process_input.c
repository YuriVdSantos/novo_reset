/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:54:45 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/27 04:24:27 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_input(t_ctx *ctx, const char **input)
{
	t_token	*tokens;
	t_cmd	*cmd;

	tokens = tokenize_input(ctx, input);
	if (!tokens)
	{
		ctx->exit_status = 2;
		// return (free_all_allocations(ctx));
	}
	if (!validate_syntax(ctx, tokens))
	{
		ctx->exit_status = 2;
		// return (free_all_allocations(ctx));
	}
	cmd = parse_tokens(ctx, &tokens);
	if (!cmd)
	{
		ctx->exit_status = 2;
		// return (free_all_allocations(ctx));
	}
	if(ctx->cmd_list && ctx->cmd_list->next)
	{
		ctx->exit_status = execute_multiple_commands(cmd, ctx);
	}
	else
	{
		ctx->exit_status = execute_one_command(cmd, ctx);
	}

	// free_all_allocations(ctx);
}
