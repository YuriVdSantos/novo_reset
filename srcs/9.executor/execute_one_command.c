

#include "../../minishell.h"

//Quando o fork é executado ele não está com o path na env, ou seja preciso garantir que a env esteja pronta quando essa chamada acontecer.

int	execute_forked_external(char **args, t_ctx *ctx)
{
    int		child_pid;
    
    // printf("PATH: '%s'\n", getenv("PATH"));
    child_pid = fork();
    define_execute_signals(child_pid);
    if (child_pid == -1)
    {
        print_error_msg("fork", args[0]);
        return (EXIT_FAILURE);
    }
    if (child_pid == 0)
        execute_external(args, ctx->env_list, ctx);
    return (wait_for_child(child_pid, TRUE, ctx));
}

static int	handle_redirects(char *command, int original_fds[2])
{
    char	redirect;

    original_fds[IN] = NO_REDIRECT;
    original_fds[OUT] = NO_REDIRECT;
    while ((redirect = get_next_redirect(command)))
    {
        if (redirect == '<' && !handle_input_redirect(command, original_fds))
            return (FAILED);
        if (redirect == '>' && !handle_output_redirect(command, original_fds))
            return (FAILED);
        if (redirect < 0)
        {
            save_original_fd_in(original_fds);
            redirect_heredoc(command, redirect);
        }
    }
    return (SUCCESS);
}

static void	restore_original_fds(int original_fds[2])
{
    if (original_fds[IN] != NO_REDIRECT)
    {
        redirect_fd(original_fds[IN], STDIN_FILENO);
        close(original_fds[IN]);
    }
    if (original_fds[OUT] != NO_REDIRECT)
    {
        redirect_fd(original_fds[OUT], STDOUT_FILENO);
        close(original_fds[OUT]);
    }
}

int	execute_one_command(t_cmd *command, t_ctx *ctx)
{
    char	**args;
    int		exit_status;
    int		original_fds[2];

    if (handle_redirects(command->args[0], original_fds) == FAILED)
    {
        restore_original_fds(original_fds);
        free(command);
        return (EXIT_FAILURE);
    }
    args = command->args;
    // free(command);
    
    if (is_builtin(args[0]))
        exit_status = execute_builtin(args, ctx, &ctx->env_list);
    else
        exit_status = execute_forked_external(args, ctx);
    // free_string_array(args);
    restore_original_fds(original_fds);
    return (exit_status);
}