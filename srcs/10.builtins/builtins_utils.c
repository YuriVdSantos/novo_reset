#include "minishell.h"

/**
 * @brief Verifica se um comando é um dos built-ins do minishell.
 * @param command O nome do comando a ser verificado.
 * @return 1 se for um built-in, 0 caso contrário.
 */
int	is_builtin(char *command)
{
	if (!command)
		return (0);
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "pwd") == 0)
		return (1);
	if (ft_strcmp(command, "export") == 0)
		return (1);
	if (ft_strcmp(command, "unset") == 0)
		return (1);
	if (ft_strcmp(command, "env") == 0)
		return (1);
	if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}

/**
 * @brief Verifica se um comando é um built-in que altera o estado do
 * shell principal e, portanto, não pode ser executado em um fork.
 * @param command O nome do comando.
 * @return 1 se for um built-in "especial", 0 caso contrário.
 */
int	is_special_builtin(char *command)
{
	if (!command)
		return (0);
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "exit") == 0)
		return (1);
	if (ft_strcmp(command, "unset") == 0)
		return (1);
	// 'export' com argumentos também deve ser tratado como especial.
	if (ft_strcmp(command, "export") == 0)
		return (1);
	return (0);
}

// =============================================================================
// Suas outras funções de builtins_utils.c devem permanecer aqui.
// Adicionei apenas as que estavam faltando.
// =============================================================================

void	move_one_forward(char *str)
{
	ft_memmove(str, str + 1, ft_strlen(str + 1) + 1);
}

int	str_equal(const char *str1, const char *str2)
{
	size_t	size;

	if (!str1 || !str2)
		return (0);
	size = ft_strlen(str1);
	if (size != ft_strlen(str2))
		return (0);
	return (ft_strcmp(str1, str2) == 0);
}
