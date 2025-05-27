

#include "minishell.h"

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	getcwd(cwd, PATH_MAX);
	ft_putstr_fd(cwd, 1);
	ft_putstr_fd("\n", 1);
	return (0);
}
