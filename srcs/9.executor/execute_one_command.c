// Em srcs/9.executor/execute_one_command.c

#include "minishell.h"

extern int g_exit_status;

int execute_one_command(t_cmd *command, t_ctx *ctx)
{
    pid_t   pid;
    int     original_fds[2];

    if (!command || !command->args || !command->args[0])
        return (g_exit_status);

    // 1. Aplicar redirecionamentos ANTES de decidir como executar.
    if (apply_redirections(command, original_fds) == FAILED)
    {
        restore_original_fds(original_fds); // Restaura em caso de falha
        return (1); // Retorna status de erro
    }

    // 2. Se for QUALQUER built-in (não apenas especial), executa no processo pai.
    if (is_builtin(command->args[0]))
    {
        g_exit_status = execute_builtin(command->args, ctx);
    }
    // 3. Se for um comando externo, cria um processo filho.
    else
    {
        pid = fork();
        if (pid == -1)
        {
            print_error(ctx, "fork", -1, 1);
            restore_original_fds(original_fds); // Não se esqueça de restaurar os fds
            return (1);
        }
        if (pid == 0) // Processo Filho
        {
            // O filho herda os fds redirecionados.
            // Apenas executa comandos externos.
            execute_external(command->args, ctx->env_list, ctx);
        }
        // Processo Pai
        g_exit_status = wait_for_child(pid, 1, ctx);
    }

    // 4. Restaurar os file descriptors originais após a execução.
    restore_original_fds(original_fds);

    return (g_exit_status);
}