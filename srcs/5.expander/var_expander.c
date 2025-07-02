/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 14:54:17 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/02 19:48:14 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*return_var_value(t_ctx *ctx, const char *var_name, int *len);

char	*get_var_value(t_ctx *ctx, const char *var_name, int *len)
{
	char	*val;
	char	*key;

	if ((var_name[0] == '?') || (var_name[0] == '$') \
		|| (var_name[0] == '\0') || ((!ft_isalpha(var_name[0])) \
		&& (var_name[0] != '_')))
		return (return_var_value(ctx, var_name, len));
	*len = 0;
	while (ft_isalnum(var_name[*len]) || var_name[*len] == '_')
		(*len)++;
	key = ft_safe_strndup(ctx, var_name, *len);
	if (!key)
		return (NULL);
	val = get_env_value(ctx, key);
	if (val)
		return (safe_strdup(ctx, val));
	return (safe_strdup(ctx, ""));
}

static char	*return_var_value(t_ctx *ctx, const char *var_name, int *len)
{
	char	*pid_str;

	if (var_name[0] == '?')
	{
		*len = 1;
		return (safe_itoa(ctx, ctx->exit_status));
	}
	if (var_name[0] == '$')
	{
		*len = 1;
		get_pid_var(ctx, &pid_str);
		return (pid_str);
	}
	if (var_name[0] == '\0' || (!ft_isalpha(var_name[0]) && var_name[0] != '_'))
	{
		*len = 0;
		return (safe_strdup(ctx, "$"));
	}
	return (NULL);
}

int	is_valid_dollar(char c)
{
	return (ft_isalnum(c) || c == '{' || c == '?' || c == '$' || c == '_');
}
