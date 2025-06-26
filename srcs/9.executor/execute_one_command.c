#include "minishell.h"

// volatile sig_atomic_t g_signal;

int execute_one_command(t_cmd *command, t_ctx *ctx)
{
    pid_t   pid;
    int     original_fds[2];
    if (!command || !command->args || !command->args[0])
        return (g_signal);
    if (apply_redirections(command, original_fds) == FAILED)
    {
        restore_original_fds(original_fds);
        return (1);
    }
    if (is_builtin(command->args[0]))
    {
        g_signal = execute_builtin(command->args, ctx);
    }
    else
    {
        pid = fork();
        if (pid == -1)
        {
            print_error(ctx, "fork", -1, 1);
            restore_original_fds(original_fds);
            return (1);
        }
        if (pid == 0)
        {
            define_non_interactive_signals();
            execute_external(command->args, ctx->env_list, ctx);
        }
		else
		{
            define_execute_signals(pid);
			g_signal = wait_for_child(pid, 1, ctx);
			define_interactive_signals();
		}
    }
    restore_original_fds(original_fds);
    return (g_signal);
}
