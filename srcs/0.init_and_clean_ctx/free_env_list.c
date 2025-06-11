/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 01:28:41 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/10 20:13:19 by yurivieirad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current != NULL)
	{
		free(current->key);
		current->key = NULL;
		free(current->value);
		current->value = NULL;
		next = current->next;
		free(current);
		current = next;
	}
	current = NULL;
}