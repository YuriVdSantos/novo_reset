#include "minishell.h"

int	g_exit_status = 0;

void	execute(t_ctx *ctx);
void	free_ctx_between_commands(t_ctx *ctx);

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
	while (1)
	{
		define_signals();
		ctx->input = readline("minishell> ");
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
	free_context(ctx);
	return (g_exit_status);
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
