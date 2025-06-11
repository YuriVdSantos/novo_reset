/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:54:45 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/11 00:20:13 by yurivieirad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_cmd(t_cmd *cmd, t_ctx *ctx);

void	process_input(t_ctx *ctx, const char **input)
{
	t_token	*tokens;
	t_cmd	*ast;

	tokens = tokenize_input(ctx, input);
	ctx->token_list = tokens;
	if (!tokens)
	{
		ctx->exit_status = 2;
		return ;
	}
	if (!validate_syntax(ctx, tokens))
	{
		ctx->exit_status = 258;
		return ;
	}
	ast = parse_tokens(ctx, &tokens);
	ctx->cmd_list = ast;
	if (!ast)
	{
		ctx->exit_status = 258;
		return ;
	}
	if (process_heredocs(ast, ctx) == FAILED)
	{
		ctx->exit_status = 1;
		return ;
	}
	return (execute_cmd(ast, ctx));
}

static void	execute_cmd(t_cmd *cmd, t_ctx *ctx)
{
	if (ctx->cmd_list && ctx->cmd_list->next)
		ctx->exit_status = execute_multiple_commands(cmd, ctx);
	else
		ctx->exit_status = execute_one_command(cmd, ctx);
}
