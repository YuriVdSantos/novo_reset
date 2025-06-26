#include "minishell.h"

extern int	g_signal;

void	print_error_msg(char *command, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (command)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(msg, STDERR_FILENO);
}


void	print_error(t_ctx *ctx, char *context_msg, int errnum, int exit_status)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    if (context_msg)
    {
        ft_putstr_fd(context_msg, STDERR_FILENO);
    }
	if (errnum > 0) 
    {
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putstr_fd(strerror(errnum), STDERR_FILENO);
    }
	ft_putstr_fd("\n", STDERR_FILENO);
    if (ctx)
        ctx->exit_status = exit_status;
    else
        g_signal = exit_status;
}


bool	handle_error(t_ctx *ctx, char *msg, int errnum, int exit_status)
{
	print_error(ctx, msg, errnum, exit_status);
	return (false);
}


bool	syntax_error(t_ctx *ctx, char *msg)
{
	char	*error_msg;

	error_msg = ft_strjoin("syntax error near unexpected token `", msg);
	error_msg = ft_strjoin_free(NULL, error_msg, "'");
	print_error(ctx, error_msg, 0, 2);
	free(error_msg);
	return (false);
}
