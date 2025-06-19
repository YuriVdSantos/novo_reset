
#include "minishell.h"

// volatile sig_atomic_t	g_signal = 0;

static char	*get_prompt(t_ctx *ctx)
{
	if (ctx->is_interactive)
		return ("minishell> ");
	else
		return ("");
}

int	main(int argc, char **argv, char **env)
{
	t_ctx	*ctx;
	int		exit_status;

	if (argc != 1)
	{
		ft_putstr_fd("Usage: ./minishell\n", 2);
		return (EXIT_FAILURE);
	}
	(void)argv;
	ctx = malloc(sizeof(t_ctx));
	if (!ctx)
		return (ft_putstr_fd("memory allocation error\n", 2), EXIT_FAILURE);
	ctx = init_ctx(ctx, env);
	if (!ctx)
		return (ft_putstr_fd("minishell: initialization error\n", 2), \
				EXIT_FAILURE);
	main_loop(ctx);
	exit_status = ctx->exit_status;
    if (ctx->is_interactive)
    {
		rl_clear_history();
    }
	free_context(ctx);
	return (exit_status);
}

void	main_loop(t_ctx *ctx)
{
	char	*prompt;

	while (1)
	{
		define_signals();
		prompt = get_prompt(ctx);
		if (ctx->is_interactive)
			ctx->input = readline(prompt);
		else
			ctx->input = get_next_line_simplified(STDIN_FILENO);
		if (ctx->input == NULL)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		if (ctx->input[0] == '\0')
		{
			free(ctx->input);
			ctx->input = NULL;
			continue ;
		}
		add_history(ctx->input);
		process_input(ctx, ctx->input);
		super_free(ctx);
		if (ctx->input)
			free(ctx->input);
		ctx->input = NULL;
	}
}
