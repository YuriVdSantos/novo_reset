/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_new_env_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 04:27:46 by codespace         #+#    #+#             */
/*   Updated: 2025/06/30 04:29:05 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_valid_env_vars(t_env *env_list)
{
	size_t	count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		if (current->value != NULL)
			count++;
		current = current->next;
	}
	return (count);
}

static void	populate_env_list_str(t_ctx *ctx)
{
	int		i;
	t_env	*current;
	char	*tmp;

	i = 0;
	current = ctx->env_list;
	while (current)
	{
		if (current->value != NULL)
		{
			tmp = ft_strjoin(current->key, "=");
			ctx->env_list_str[i++] = ft_strjoin(tmp, current->value);
			free(tmp);
		}
		current = current->next;
	}
	ctx->env_list_str[i] = NULL;
}

void	sync_env_list_str(t_ctx *ctx)
{
	size_t	count;

	if (ctx->env_list_str)
		free_string_array(ctx->env_list_str);
	count = count_valid_env_vars(ctx->env_list);
	ctx->env_list_str = malloc(sizeof(char *) * (count + 1));
	if (!ctx->env_list_str)
		return ;
	populate_env_list_str(ctx);
}
