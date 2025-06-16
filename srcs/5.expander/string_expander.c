/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 23:23:07 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/15 23:25:47 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define EXPAND_BUFFER_SIZE 4096

char	*expand_string(t_ctx *ctx, const char *input)
{
	char	result[EXPAND_BUFFER_SIZE];
	char	*cursor;
	char	*var_val;
	int		len;

	ft_bzero(result, EXPAND_BUFFER_SIZE);
	cursor = result;
	while (*input)
	{
		if (*input == '$' && is_valid_dollar(input[1]))
		{
			var_val = expand_env_var(ctx, input + 1, &len);
			ft_strlcpy(cursor, var_val, EXPAND_BUFFER_SIZE - (cursor - result));
			cursor += ft_strlen(var_val);
			input += len + 1;
			if (ft_strcmp(var_val, "?") == 0 || ft_strcmp(var_val, "$") == 0)
				free(var_val);
		}
		else
		{
			*cursor++ = *input++;
			if (cursor >= result + EXPAND_BUFFER_SIZE - 1)
				break ;
		}
	}
	*cursor = '\0';
	return (safe_strdup(ctx, result));
}

int	is_valid_dollar(char c)
{
	return (ft_isalnum(c) || c == '{' || c == '?' || c == '$' || c == '_');
}
