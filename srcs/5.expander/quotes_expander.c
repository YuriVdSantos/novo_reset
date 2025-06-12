/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:15:27 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/10 23:17:05 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
# define BUFFER_SIZE 2025

char	*expand_dquotes(t_ctx *ctx, const char *input)
{
	char	*result;
	char	*cursor;
	char	*var;
	int		len;

	result = safe_malloc(ctx, BUFFER_SIZE, ALLOC_TYPE_STRING);
	cursor = result;
	while (*input)
	{
		if (*input == '$' && is_valid_dollar(input[1]))
		{
			var = expand_env_var(ctx, input + 1, &len);
			ft_strlcpy(cursor, var, BUFFER_SIZE - (cursor - result));
			cursor += ft_strlen(var);
			input += len + 1;
			safe_free(ctx, var);
		}
		else
			*cursor++ = *input++;
	}
	*cursor = '\0';
	return (result);
}
