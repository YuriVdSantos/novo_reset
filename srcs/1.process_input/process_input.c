/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:54:45 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/09 16:38:35 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		ctx->exit_status = 2;
		return ;
	}
	ast = parse_tokens(ctx, &tokens);
	ctx->cmd_list = ast;
	if (!ast)
	{
		ctx->exit_status = 2;
		return ;
	}
	if (ctx->cmd_list && ctx->cmd_list->next)
		ctx->exit_status = execute_multiple_commands(ast, ctx);
	else
		ctx->exit_status = execute_one_command(ast, ctx);
}
