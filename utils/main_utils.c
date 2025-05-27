/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:32:53 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/26 18:03:54 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_minishell(t_ctx *ctx, char **input)
{
	add_history(*input);
	process_input(ctx, (const char **)input);
}

void	no_input(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "exit\\n", 5);
}

void	input_null(t_ctx *ctx, char **input)
{
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	free_all_allocations(ctx);
	exit(ctx->exit_status);
	free(*input);
}
