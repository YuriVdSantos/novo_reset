/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ctx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:55:10 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/26 19:37:38 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_with_equal(t_ctx *ctx, t_env *new_node, const char *env_str)
{
	char	*equals_pos;
	size_t	key_len;

	equals_pos = ft_strchr(env_str, '=');
	key_len = equals_pos - env_str;
	new_node->key = safe_malloc(ctx, key_len + 1, ALLOC_TYPE_STRING);
	ft_strlcpy(new_node->key, env_str, key_len + 1);
	new_node->value = safe_strdup(ctx, equals_pos + 1);
}

static void	handle_without_equal(t_ctx *ctx, t_env *new_node, \
	const char *env_str)
{
	new_node->key = safe_strdup(ctx, env_str);
	new_node->value = safe_strdup(ctx, "");
}

void	add_env_node(t_ctx *ctx, t_env **list_head, const char *env_str)
{
	t_env	*new_node;
	t_env	*current;

	new_node = safe_malloc(ctx, sizeof(t_env), ALLOC_TYPE_ENV_NODE);
	if (!new_node)
		return ;
	if (ft_strchr(env_str, '='))
		handle_with_equal(ctx, new_node, env_str);
	else
		handle_without_equal(ctx, new_node, env_str);
	new_node->next = NULL;
	if (*list_head == NULL)
		*list_head = new_node;
	else
	{
		current = *list_head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

void	init_environment(t_ctx *ctx, char **env_array)
{
	int		i;
	char	*cwd;

	i = 0;
	while (env_array[i])
	{
		add_env_node(ctx, &ctx->env_list, env_array[i]);
		i++;
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ctx->pwd = safe_strdup(ctx, cwd);
		free(cwd);
	}
	else
	{
		ctx->pwd = safe_strdup(ctx, "");
		print_error(ctx, "getcwd failed", errno, 1);
	}
	ctx->oldpwd = safe_strdup(ctx, "");
}

t_ctx	*init_ctx(t_ctx *ctx, char **env_array)
{
	ctx->env_list = NULL;
	ctx->cmd_list = NULL;
	ctx->token_list = NULL;
	ctx->input = NULL;
	ctx->exit_status = 0;
	ctx->previous_exit_status = 0;
	ctx->current_exit_status = 0;
	ctx->last_error_message = NULL;
	ctx->allocations = malloc(sizeof(t_allocation));
	if (!ctx->allocations)
	{
		ft_putstr_fd("minishell: initialization error\n", 2);
		return (NULL);
	}
	ctx->pwd = NULL;
	ctx->oldpwd = NULL;
	ctx->is_interactive = isatty(STDIN_FILENO);
	init_environment(ctx, env_array);
	//setup_signals();
	return (ctx);
}
