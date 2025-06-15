/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:23:43 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/15 20:23:04 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_assignment_var(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	char	*value;
	int		i;

	i = 0;
	while ((*tmp)->value && (*tmp)->value[i] && (*tmp)->value[i] != '=')
		i++;
	value = ft_strdup((*tmp)->value + i + 1);
	add_arg(current, value);
	set_env_var(ctx, (*tmp)->value);
	*tmp = (*tmp)->next;
}

void	handle_env_var(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	char	*env_value;

	//se for unset tem que mandar a key não o o valor
	env_value = safe_strdup(ctx, get_env_value(ctx, (*tmp)->value));
	add_arg(current, env_value);
	current->type = CMD_BUILTIN;
	*tmp = (*tmp)->next;
}

void	handle_parse_error(t_ctx *ctx, t_token **tmp)
{
	print_error(ctx, "syntax error", -1, 2);
	*tmp = (*tmp)->next;
}
