/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_new_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:01:05 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/30 04:28:11 by codespace        ###   ########.fr       */
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

void	add_new_env_var(t_ctx *ctx, char *key, char *value)
{
	t_env	*new_node;
	t_env	*current;

	(void)ctx;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	if (!ctx->env_list)
		ctx->env_list = new_node;
	else
	{
		current = ctx->env_list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

void	update_existing_var(t_ctx *ctx, t_env *var, const char *value)
{
	(void)ctx;
	if (var->value)
		free(var->value);
	var->value = ft_strdup(value);
}

static void	extract_key_value(const char *assignment, char **key, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(assignment, '=');
	if (equal_pos)
	{
		*key = ft_strndup(assignment, equal_pos - assignment);
		*value = ft_strdup(equal_pos + 1);
	}
	else
	{
		*key = ft_strdup(assignment);
		*value = NULL;
	}
}

void	set_env_var(t_ctx *ctx, const char *assignment)
{
	char	*key;
	char	*value;
	t_env	*existing_var;

	extract_key_value(assignment, &key, &value);
	existing_var = find_env_var(ctx->env_list, key);
	if (existing_var)
	{
		if (value)
			update_existing_var(ctx, existing_var, value);
		free(key);
		if (value)
			free(value);
	}
	else
		add_new_env_var(ctx, key, value);
	sync_env_list_str(ctx);
}
