
#include "minishell.h"

int	execute_forked_external(char **args, t_ctx *ctx)
{
	int		child_pid;

	child_pid = fork();
	define_execute_signals(child_pid);
	if (child_pid == -1)
	{
		print_error(ctx, "fork", errno, 1);
		return (EXIT_FAILURE);
	}
	if (child_pid == 0)
		execute_external(args, ctx->env_list, ctx);
	return (wait_for_child(child_pid, TRUE, ctx));
}

int	execute_one_command(t_cmd *command, t_ctx *ctx)
{
	char	**args;
	int		exit_status;
	int		original_fds[2];

	if (apply_redirections(command, original_fds) == FAILED)
	{
		restore_original_fds(original_fds);
		return (EXIT_FAILURE);
	}
	args = command->args;
	if (only_var_assignments(ctx->token_list))
		exit_status = 0;
	else if (is_builtin(args[0]))
		exit_status = execute_builtin(args, ctx);
	else
		exit_status = execute_forked_external(args, ctx);
	restore_original_fds(original_fds);
	return (exit_status);
}


