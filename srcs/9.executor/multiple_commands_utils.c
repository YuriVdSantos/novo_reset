/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_commands_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:32:19 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/27 15:32:47 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	arr_len(char **arr)
{
	int	len;

	len = 0;
	while (*arr)
	{
		len++;
		arr++;
	}
	return (len);
}

int	*init_children_pid(char **commands)
{
	int		*children_pid;
	size_t	num_commands;

	num_commands = arr_len(commands);
	children_pid = ft_calloc(num_commands + 1, sizeof(int));
	if (!children_pid)
		return (NULL);
	return (children_pid);
}

void	clean_after_execute(int *children_pid)
{
	if (children_pid)
	{
		close_extra_fds();
	}
}

void	quit_child(char **commands, t_ctx *ctx)
{
	if (commands)
		if (ctx->env_list)
			close_all_fds();
	rl_clear_history();
	exit(EXIT_FAILURE);
}
