/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 01:28:41 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/27 01:51:00 by jhualves         ###   ########.fr       */
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
