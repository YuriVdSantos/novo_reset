#include "minishell.h"

/**
 * @brief Verifica se um comando é um built-in que DEVE rodar no processo pai.
 * @param cmd O nome do comando a ser verificado.
 * @return Retorna 1 se for um built-in especial, 0 caso contrário.
 */
int	is_special_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	// 'export' sem argumentos pode rodar no filho, mas com argumentos deve
	// rodar no pai para modificar o ambiente. Por simplicidade aqui,
	// tratamos todos os 'export' como especiais.
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	return (0);
}

/**
 * @brief Libera a memória de um array de strings terminado por NULL.
 * @param arr O array a ser liberado.
 */
void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
