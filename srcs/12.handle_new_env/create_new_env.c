/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_new_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 21:10:41 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/26 03:13:36 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_env_var(t_env *env_list, const char *key)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	update_existing_var(t_ctx *ctx, t_env *var, const char *value)
{
	safe_free(ctx, var->value);
	var->value = safe_strdup(ctx, value);
}

void	add_new_env_var(t_ctx *ctx, t_env **env_list, char *key, char *value)
{
	t_env	*new_node;
	t_env	*current;

	new_node = safe_malloc(ctx, sizeof(t_env), ALLOC_TYPE_ENV_NODE);
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	if (!*env_list)
		*env_list = new_node;
	else
	{
		current = *env_list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

void	set_env_var(t_ctx *ctx, const char *assignment)
{
	char	*equal_pos;
	char	*key;
	char	*value;
	t_env	*existing_var;

	equal_pos = safe_strchr(ctx, assignment, '=');
	if (!equal_pos || equal_pos == assignment)
		return ;
	key = ft_safe_strndup(ctx, assignment, equal_pos - assignment);
	value = safe_strdup(ctx, equal_pos + 1);
	existing_var = find_env_var(ctx->env_list, key);
	if (existing_var)
		update_existing_var(ctx, existing_var, value);
	else
		add_new_env_var(ctx, &ctx->env_list, key, value);
	safe_free(ctx, key);
}
