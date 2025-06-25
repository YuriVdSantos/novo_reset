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

static int	is_within_long_long_range(const char *str)
{
	int			len;
	const char	*limit;
	int			is_negative;

	is_negative = 0;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			is_negative = 1;
		str++;
	}
	len = ft_strlen(str);
	if (is_negative)
		limit = "9223372036854775808";
	else
		limit = "9223372036854775807";
	if (len > (int)ft_strlen(limit))
		return (0); // Excede o comprimento máximo
	if (len == (int)ft_strlen(limit) && ft_strcmp(str, limit) > 0)
		return (0); // Excede o valor máximo/mínimo
	return (1);
}

int	ft_exit(char **args, t_ctx *ctx)
{
	long long	status;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args[1])
	{
		status = ctx->exit_status;
		free_context(ctx);
		exit(status);
	}
	// Validação combinada de formato e intervalo
	if (!is_numeric(args[1]) || !is_within_long_long_range(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		free_context(ctx);
		exit(2); // Bash usa o status 2 para este erro
	}
	else if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1); // Retorna 1 mas não sai do shell
	}
	else
	{
		status = ft_atodbl(args[1]); // ft_atodbl é seguro aqui pois já validamos
	}
	free_context(ctx);
	exit((unsigned char)status);
}
