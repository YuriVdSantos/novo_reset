/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:07:06 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/01 17:21:36 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			execute(t_ctx *ctx);
void			free_ctx_between_commands(t_ctx *ctx);

volatile sig_atomic_t	g_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_ctx	*ctx;

	(void)argc;
	(void)argv;
	ctx = ft_calloc(1, sizeof(t_ctx));
	if (!ctx)
	{
		perror("ft_calloc failed");
		exit(EXIT_FAILURE);
	}
	init_ctx(ctx, envp);
	ctx = main_loop(ctx);
	close_all_fds();
	free_context(ctx);
	return (g_signal);
}

t_ctx	*main_loop(t_ctx *ctx)
{
	while (1)
	{
		define_signals();
		ctx->input = readline("minishell> ");
		if (g_signal == 130)
		{
			ctx->exit_status = 130;
			g_signal = 0;
		}
		if (!ctx->input)
		{
			printf("exit\n");
			break ;
		}
		if (ft_strlen(ctx->input) > 0)
			add_history(ctx->input);
		process_input(ctx, ctx->input);
		free_ctx_between_commands(ctx);
	}
	return (ctx);
}

void	execute(t_ctx *ctx)
{
	if (!ctx || !ctx->cmd_list)
		return ;
	if (!ctx->cmd_list->next)
		execute_one_command(ctx->cmd_list, ctx);
	else
		execute_multiple_commands(ctx->cmd_list, ctx);
}

void	free_ctx_between_commands(t_ctx *ctx)
{
	if (!ctx)
		return ;
	if (ctx->input)
	{
		free(ctx->input);
		ctx->input = NULL;
	}
	if (ctx->token_list)
	{
		free_token_list(ctx->token_list);
		ctx->token_list = NULL;
	}
	if (ctx->cmd_list)
	{
		free_cmd_list(ctx->cmd_list);
		ctx->cmd_list = NULL;
	}
}
