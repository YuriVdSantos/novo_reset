/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_new_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 21:10:41 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/15 19:33:11 by jhualves         ###   ########.fr       */
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
	int		i;

	i = 0;
	free(var->value);
	var->value = ft_strdup(value);
	if (!var->value)
		handle_error(ctx, \
			"Memory allocation failed for environment variable value.", 12, 1);
	else
	{
		while (ctx->env_list_str[i] && ft_strncmp(ctx->env_list_str[i], \
			var->key, ft_strlen(var->key)) != 0)
			i++;
		ctx->env_list_str[i] = ft_strjoin(var->key, "=");
		ctx->env_list_str[i] = ft_strjoin(ctx->env_list_str[i], value);
	}
}

void	add_new_env_var(t_ctx *ctx, char *key, char *value, \
	const char *assignment)
{
	t_env	*new_node;
	t_env	*current;
	int		i;

	i = 0;
	new_node = malloc(sizeof(t_env));
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
	while (ctx->env_list_str && ctx->env_list_str[i] != NULL)
		i++;
	ctx->env_list_str = realloc(ctx->env_list_str, sizeof(char *) * (i + 2));
	if (!ctx->env_list_str)
		handle_error(ctx, "Memory allocation failed.", 12, 1);
	ctx->env_list_str[i] = ft_strdup(assignment);
	ctx->env_list_str[i + 1] = NULL;
}

void	set_env_var(t_ctx *ctx, const char *assignment)
{
	char	*equal_pos;
	char	*key;
	char	*value;
	t_env	*existing_var;
	size_t	key_len;

	key_len = 0;
	equal_pos = safe_strchr(ctx, assignment, '=');
	if (!equal_pos || equal_pos == assignment)
		return ;
	while (assignment[key_len] && assignment[key_len] != '=')
		key_len++;
	if (key_len == 0)
		return ;
	key = ft_strndup(assignment, key_len);
	value = ft_strdup(equal_pos + 1);
	existing_var = find_env_var(ctx->env_list, key);
	if (existing_var)
		update_existing_var(ctx, existing_var, value);
	else
		add_new_env_var(ctx, key, value, assignment);
}

void	unset_env_var(t_ctx *ctx, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = ctx->env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				ctx->env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			break ;
		}
		prev = current;
		current = current->next;
	}
	unset_string_env_var(ctx, key);
}
