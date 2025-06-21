#include "minishell.h"

static int	is_numeric_string(const char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	ft_exit(char **args, t_ctx *ctx)
{
	int	exit_status;

	if (!isatty(STDIN_FILENO))
    	ft_putstr_fd("exit\n", STDOUT_FILENO);

	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		ctx->exit_status = 1;
		return (EXIT_FAILURE);
	}
	if (args[1])
	{
		if (!is_numeric_string(args[1]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit_status = 2;
		}
		else
		{
			exit_status = ft_atoi(args[1]);
		}
	}
	else
	{
		exit_status = ctx->exit_status;
	}
	rl_clear_history();
	free_context(ctx);
	exit(ctx->exit_status = exit_status);
}