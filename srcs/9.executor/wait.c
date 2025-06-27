/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:41:02 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/27 15:41:34 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// volatile sig_atomic_t	g_signal;

int	wait_for_child(int child_pid, int is_last_child, t_ctx *ctx)
{
	int	status;

	(void)ctx;
	if (waitpid(child_pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFEXITED(status))
		g_signal = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_signal = 128 + WTERMSIG(status);
		if (is_last_child)
		{
			if (WTERMSIG(status) == SIGQUIT)
				ft_putendl_fd("Quit: 3", STDERR_FILENO);
			else if (WTERMSIG(status) == SIGINT)
				ft_putstr_fd("\n", STDERR_FILENO);
			else if (WTERMSIG(status) == SIGPIPE)
				ft_putendl_fd("Broken pipe", STDERR_FILENO);
		}
	}
	return (g_signal);
}

int	wait_for_children(int children_pid[1024], t_ctx *ctx)
{
	int	i;
	int	last_status;
	int	is_last;

	i = 0;
	last_status = 0;
	while (children_pid && children_pid[i] != 0)
	{
		is_last = (children_pid[i + 1] == 0);
		last_status = wait_for_child(children_pid[i], is_last, ctx);
		i++;
	}
	return (last_status);
}
