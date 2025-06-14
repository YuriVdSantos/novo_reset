/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:09:55 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/13 21:35:09 by yvieira-         ###   ########.fr       */
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
	char	*free_input;

	while (1)
	{
		define_signals();
		prompt = get_prompt(ctx);
		input = readline(prompt);
		free_input = input;
		if (input == NULL)
		{
			input_null(ctx, &input);
			break ;
		}
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		process_input(ctx, (const char **)&input);
		super_free(ctx);
		free(free_input);
	}
}
