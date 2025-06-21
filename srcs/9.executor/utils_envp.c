#include "minishell.h"

size_t	minienv_size(t_env *minienv)
{
	size_t	size;
	t_env	*aux;

	size = 0;
	aux = minienv;
	while (aux)
	{
		size++;
		aux = aux->next;
	}
	return (size);
}

char	**minienv_to_envp(t_env *minienv)
{
	char	**envp;
	t_env	*aux;
	int		i;
	char	*tmp;

	// O size aqui deve ser apenas das vars com valor.
	envp = malloc(sizeof(char *) * (minienv_size(minienv) + 1));
	aux = minienv;
	i = 0;
	while (aux)
	{
		if (aux->value) // Enviar para execve apenas vars com valor
		{
			tmp = ft_strjoin(aux->key, "=");
			envp[i] = ft_strjoin(tmp, aux->value);
			free(tmp);
			i++;
		}
		aux = aux->next;
	}
	envp[i] = NULL;
	return (envp);
}