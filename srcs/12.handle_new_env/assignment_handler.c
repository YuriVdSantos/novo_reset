/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignment_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 01:16:01 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/15 16:05:27 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_single_assignment(t_ctx *ctx, char *arg)
{
	char	*equal_pos;

	equal_pos = safe_strchr(ctx, arg, '=');
	if (equal_pos && (equal_pos != arg))
		set_env_var(ctx, arg);
}

void	process_assignments(t_ctx *ctx, t_cmd *cmd)
{
	int		i;

	i = 0;
	if (!cmd || !cmd->args)
		return ;
	while (cmd->args[i])
	{
		process_single_assignment(ctx, cmd->args[i]);
		i++;
	}
}

void	unset_string_env_var(t_ctx *ctx, const char *key)
{
	int		i;
	char	**temp;

	i = 0;
	while (ctx->env_list_str && ctx->env_list_str[i])
		i++;
	temp = malloc(sizeof(char *) * i);
	if (!temp)
		handle_error(ctx, "Memory allocation failed.", 12, 1);
	i = 0;
	while (ctx->env_list_str && ctx->env_list_str[i])
	{
		if (ft_strncmp(ctx->env_list_str[i], key, ft_strlen(key)) != 0)
			temp[i] = ft_strdup(ctx->env_list_str[i]);
		else
		{
			ctx->env_list_str[i] = NULL;
			i++;
		}
		i++;
	}
	free_string_array(ctx->env_list_str);
	ctx->env_list_str = temp;
}
