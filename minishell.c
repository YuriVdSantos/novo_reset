/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:09:55 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/06 00:36:35 by yurivieirad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	free_context(ctx);
	rl_clear_history();
	return (exit_status);
}

void	main_loop(t_ctx *ctx)
{
	char	*prompt;
	char	*input;

	while (1)
	{
		
		prompt = get_prompt(ctx);
		input = readline(prompt);
		define_signals();
		if (!input)
		{
			no_input();
			break ;
		}
		else if (input == NULL)
		{
			input_null(ctx, &input);
			continue ;
		}
		process_minishell(ctx, &input);
		super_free(ctx);
	}
}
