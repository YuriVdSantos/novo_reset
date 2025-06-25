
#include "minishell.h"

static void	child_process_logic(t_cmd *cmd, t_ctx *ctx)
{
	int	original_fds[2];
	int exit_status;

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

int	execute_multiple_commands(t_cmd *cmd_list, t_ctx *ctx)
{
	int		children_pid[1024];
	int		fds[2];
	int		i;
	t_cmd	*current;
	int		prev_pipe_read_end;

	i = 0;
	ft_bzero(children_pid, sizeof(int) * 1024);
	prev_pipe_read_end = STDIN_FILENO;
	current = cmd_list;
	while (current)
	{
		if (current->next != NULL)
			if (pipe(fds) == -1)
				return (print_error(ctx, "pipe", errno, 1), EXIT_FAILURE);
		children_pid[i] = fork();
		define_execute_signals(children_pid[i]);
		if (children_pid[i] == 0)
		{
			if (prev_pipe_read_end != STDIN_FILENO)
			{
				redirect_fd(prev_pipe_read_end, STDIN_FILENO);
				close(prev_pipe_read_end);
			}
			if (current->next != NULL)
			{
				close(fds[0]);
				redirect_fd(fds[1], STDOUT_FILENO);
				close(fds[1]);
			}
			child_process_logic(current, ctx);
		}
		if (prev_pipe_read_end != STDIN_FILENO)
			close(prev_pipe_read_end);
		if (current->next != NULL)
		{
			close(fds[1]);
			prev_pipe_read_end = fds[0];
		}
		current = current->next;
		i++;
	}
	return (wait_for_children(children_pid, ctx));
}