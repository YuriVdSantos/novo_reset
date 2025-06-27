/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:32:53 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/26 22:02:11 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	**dup_mtz(char **mtz)
{
	int		i;
	char	**dup;

	if (!mtz)
		return (NULL);
	i = 0;
	while (mtz[i])
		i++;
	dup = malloc(sizeof(char *) * (i + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (mtz[i])
	{
		dup[i] = ft_strdup(mtz[i]);
		if (!dup[i])
		{
			free_string_array(dup);
			return (NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}
