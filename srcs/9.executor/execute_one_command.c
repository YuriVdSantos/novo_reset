#include "minishell.h"

extern int g_exit_status;

int execute_one_command(t_cmd *command, t_ctx *ctx)
{
    pid_t   pid;
    int     original_fds[2];
    if (!command || !command->args || !command->args[0])
        return (g_exit_status);
    if (apply_redirections(command, original_fds) == FAILED)
    {
        restore_original_fds(original_fds);
        return (1);
    }
    if (is_builtin(command->args[0]))
    {
        g_exit_status = execute_builtin(command->args, ctx);
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
            execute_external(command->args, ctx->env_list, ctx);
        }
        g_exit_status = wait_for_child(pid, 1, ctx);
    }
    restore_original_fds(original_fds);
    return (g_exit_status);
}
