#include "minishell.h"

static int	is_numeric(char *str)
{
	if (!str)
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
	long long	status;

	close_all_fds();
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args[1])
	{
		status = ctx->exit_status;
		free_context(ctx);
		exit(status);
	}
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		status = 2;
	}
	else if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	else
	{
		status = ft_atodbl(args[1]);
	}
	free_context(ctx);
	exit((unsigned char)status);
}
