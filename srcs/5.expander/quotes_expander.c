/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 21:41:14 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/01 16:33:25 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*return_null_error(t_ctx *ctx);

char	*expand_dquotes(t_ctx *ctx, const char *input)
{
	char	*result;
	char	*env_value;
	int		len;

	result = safe_strdup(ctx, "");
	if (!result)
		return (NULL);
	while (*input)
	{
		if (*input == '$')
		{
			env_value = get_var_value(ctx, input + 1, &len);
			if (!env_value)
				return (NULL);
			result = safe_strjoin(ctx, result, env_value);
			input += len + 1;
		}
		else
			result = safe_strjoin(ctx, result, (char []){*input++, '\0'});
	}
	if (result == NULL)
		return (return_null_error(ctx));
	return (result);
}

static void	*return_null_error(t_ctx *ctx)
{
	handle_error(ctx, "Memory allocation error in expand_dquotes", 12, 1);
	return (NULL);
}

char	*get_env_value(t_ctx *ctx, const char *key)
{
	t_env	*node;

	if (!key || ft_strcmp(key, "") == 0)
		return (NULL);
	node = find_env_var(ctx->env_list, key);
	if (node)
		return (node->value);
	return (NULL);
}

int	var_name_length(const char *input)
{
	int	len;

	len = 0;
	if (input[0] == '?')
		return (1);
	if (input[0] == '$')
		return (1);
	while (input[len] && (ft_isalpha_upper(input[len]) || \
ft_isdigit(input[len]) || input[len] == '_'))
		len++;
	return (len);
}

char	*expand_env_var(t_ctx *ctx, const char *input, int *len)
{
	return (get_var_value(ctx, input, len));
}
