/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:15:27 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/13 21:42:10 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_isalpha_upper(int c);

char	*expand_dquotes(t_ctx *ctx, const char *input)
{
	char	*result;
	char	*env_value;
	int		len;

	result = ft_strdup("");
	while (*input)
	{
		if (*input == '$')
		{
			input++;
			len = var_name_length(input);
			env_value = expand_env_var(ctx, input, &len);
			result = ft_strjoin_free(ctx, result, env_value);
			input += len;
			free(env_value);
		}
		else
			result = ft_strjoin_free(ctx, result, (char []){*input++, '\0'});
	}
	if (result == NULL)
	{
		handle_error(ctx, "Memory allocation error in expand_dquotes", 12, 1);
		return (NULL);
	}	
	return (result);
}

char	*get_env_value(t_ctx *ctx, const char *key)
{
	t_env	*node;
	char	*pid_str;

	pid_str = NULL;
	if (!key)
		return ("");
	if (ft_strcmp(key, "?"))
		return (safe_itoa(ctx, ctx->exit_status));
	if (ft_strcmp(key, "$"))
	{
		get_pid_var(&pid_str);
		return (pid_str);
	}
	node = find_env_var(ctx->env_list, key);
	if (node && node->value)
		return (node->value);
	return ("");
}

int	var_name_length(const char *input)
{
	int	len;

	len = 0;
	while (input[len] && (ft_isalpha_upper(input[len]) || \
	ft_isdigit(input[len]) || input[len] == '_'))
		len++;
	return (len);
}

char	*expand_env_var(t_ctx *ctx, const char *input, int *len)
{
	char	*key;

	key = ft_substr(input, 0, *len);
	if (!key)
	{
		handle_error(ctx, "Memory allocation error in expand_env_var", 12, 1);
		return (NULL);
	}
	return (get_env_value(ctx, key));
}

static int	ft_isalpha_upper(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	else
		return (0);
}
