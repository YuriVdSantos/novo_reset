#include "../../minishell.h"

int	execute_forked_builtin(char **args, t_ctx *ctx, t_env **minienv)
{
    int	exit_status;

    exit_status = execute_builtin(args, ctx, minienv);
    exit(exit_status);
}

int	execute_builtin(char **args, t_ctx *ctx, t_env **minienv)
{
	char	*command;

	command = args[0];
	if (str_equal(command, "echo"))
		return (ft_echo(args, ctx));
	if (str_equal(command, "pwd"))
		return (ft_pwd());
	if (str_equal(command, "env"))
		return (ft_env(ctx));
	if (str_equal(command, "export"))
		return (ft_export(args, ctx));
	if (str_equal(command, "unset"))
		return (ft_unset(args, minienv));
	if (str_equal(command, "cd"))
		return (ft_cd(args, ctx));
	if (str_equal(command, "exit"))
		return (ft_exit(args));
	else
		return (EXIT_FAILURE);
}
