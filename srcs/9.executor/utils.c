#include "minishell.h"

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
