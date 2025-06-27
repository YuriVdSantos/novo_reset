/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:37:20 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/27 15:37:27 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	envp = malloc(sizeof(char *) * (minienv_size(minienv) + 1));
	aux = minienv;
	i = 0;
	while (aux)
	{
		if (aux->value)
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
