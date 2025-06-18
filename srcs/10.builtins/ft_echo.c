
#include "minishell.h"
#include <stdbool.h>

static bool	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (false);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (i > 1);
}

int	ft_echo(char **args)
{
	int		i;
	bool	newline;

	i = 1;
	newline = true;

	while (args[i] && is_n_flag(args[i]))
	{
		newline = false;
		i++;
	}

	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}

	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
