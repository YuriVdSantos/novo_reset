#include "../../minishell.h"

static void	save_original_fds(int original_fds[2])
{
    if ((original_fds[0] = dup(STDIN_FILENO)) == -1 || 
        (original_fds[1] = dup(STDOUT_FILENO)) == -1)
    {
        perror("dup");
        exit(EXIT_FAILURE);
    }
}

static void	handle_redirects(t_cmd *command, t_env *minienv)
{
    char	redirect;
	char	*cmd;

	cmd = command->args[0];
    while ((redirect = get_next_redirect(cmd)))
	{
        if (redirect == '<')
        {
            if (redirect_input(cmd) == FAILED)
            {
                free_cmd_list(command);
                free_env_list(minienv);
                exit(EXIT_FAILURE);
            }
        }
        else if (redirect == '>')
        {
            if (redirect_output(cmd) == FAILED)
            {
                free_cmd_list(command);
                free_env_list(minienv);
                exit(EXIT_FAILURE);
            }
        }
    }
}

static void	execute_forked_command(t_cmd *command, t_cmd *cmd_list, t_ctx *ctx)
{
	char	**args;

	args = command->args;
	close_extra_fds();
	if (is_builtin(args[0]))
		execute_forked_builtin(args, ctx, &ctx->env_list);
	else
		execute_external(args, ctx->env_list,  ctx);
	free_cmd_list(cmd_list);
}

static void	restore_original_fds(int original_fds[2])
{
	redirect_fd(original_fds[IN], STDIN_FILENO);
	redirect_fd(original_fds[OUT], STDOUT_FILENO);
}

int	execute_multiple_commands(t_cmd *cmd, t_ctx *ctx)
{
    int	original_fds[2];
    int	children_pid[1024];
    int	i = 0;
    t_cmd *current = cmd;

    save_original_fds(original_fds);
    while (current)
    {
        children_pid[i] = fork();
        define_execute_signals(children_pid[i]);
        if (children_pid[i] == -1)
            print_error_msg("fork", current->args[0]);
        else if (children_pid[i] == 0)
        {
            handle_redirects(current, ctx->env_list);
            execute_forked_command(current, cmd, ctx);
        }
        current = current->next;
        i++;
    }
    children_pid[i] = 0; 
    restore_original_fds(original_fds);
    return (wait_for_children(children_pid, ctx));
}