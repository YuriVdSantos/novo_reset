/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_commands.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 20:45:30 by yvieira-          #+#    #+#             */
/*   Updated: 2025/07/02 20:45:34 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process_logic(t_cmd *cmd, t_ctx *ctx)
{
	int	original_fds[2];
	int	exit_status;

	if (apply_redirections(cmd, original_fds) == FAILED)
	{
		free_context(ctx);
		exit(EXIT_FAILURE);
	}
	if (is_builtin(cmd->args[0]))
	{
		exit_status = execute_builtin(cmd->args, ctx);
		free_context(ctx);
		exit(exit_status);
	}
	else
		execute_external(cmd->args, ctx->env_list, ctx);
}

static void	handle_child_process(t_cmd *cmd, t_ctx *ctx, int *fds,
		int prev_pipe_read_end)
{
	if (prev_pipe_read_end != STDIN_FILENO)
	{
		redirect_fd(prev_pipe_read_end, STDIN_FILENO);
		close(prev_pipe_read_end);
	}
	if (cmd->next != NULL)
	{
		close(fds[0]);
		redirect_fd(fds[1], STDOUT_FILENO);
		close(fds[1]);
	}
	child_process_logic(cmd, ctx);
}

static void	handle_parent_process(t_cmd *cmd, int *fds, int *prev_pipe_read_end)
{
	if (*prev_pipe_read_end != STDIN_FILENO)
		close(*prev_pipe_read_end);
	if (cmd->next != NULL)
	{
		close(fds[1]);
		*prev_pipe_read_end = fds[0];
	}
}

int	execute_multiple_commands(t_cmd *cmd_list, t_ctx *ctx)
{
	int		children_pid[1024];
	int		fds[2];
	t_cmd	*current;
	int		i;
	int		prev_pipe_read_end;

	i = 0;
	ft_bzero(children_pid, sizeof(int) * 1024);
	prev_pipe_read_end = STDIN_FILENO;
	current = cmd_list;
	while (current)
	{
		if (current->next != NULL && pipe(fds) == -1)
			return (print_error(ctx, "pipe", errno, 1), EXIT_FAILURE);
		children_pid[i] = fork();
		define_execute_signals(children_pid[i]);
		if (children_pid[i] == 0)
			handle_child_process(current, ctx, fds, prev_pipe_read_end);
		handle_parent_process(current, fds, &prev_pipe_read_end);
		current = current->next;
		i++;
	}
	return (wait_for_children(children_pid, ctx));
}
