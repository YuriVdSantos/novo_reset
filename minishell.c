/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:09:55 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/16 23:30:50 by yurivieirad      ###   ########.fr       */
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
    if (ctx->is_interactive)
    {
        rl_clear_history();
    }
	return (exit_status);
}

void	main_loop(t_ctx *ctx)
{
	char	*prompt;
	char	*input;

	while (1)
	{
		define_signals();
		prompt = get_prompt(ctx);
        if (ctx->is_interactive)
        {
            input = readline(prompt);
        }
		else
		{
            input = get_next_line_simplified(STDIN_FILENO);
        }

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
		free(input);
	}
}

