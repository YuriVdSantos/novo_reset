/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 21:55:16 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/01 17:11:58 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_expand_buffer(t_expand_buffer *buf, char *result, size_t size)
{
	buf->result = result;
	buf->cursor = result;
	buf->remaining = size;
}

static void	handle_dollar_expansion(t_ctx *ctx, const char *input, \
		int *i, t_expand_buffer *buf)
{
	int		len;
	char	*var_value;

	len = 0;
	var_value = get_var_value(ctx, &input[*i + 1], &len);
	if (!var_value)
	{
		*i += len + 1;
		return ;
	}
	if (ft_strlen(var_value) < buf->remaining)
	{
		ft_strlcpy(buf->cursor, var_value, buf->remaining);
		buf->cursor += ft_strlen(var_value);
		buf->remaining -= ft_strlen(var_value);
	}
	*i += len + 1;
}

static void	handle_regular_char(const char *input, int *i, t_expand_buffer *buf)
{
	if (buf->remaining > 1)
	{
		*buf->cursor = input[*i];
		buf->cursor++;
		buf->remaining--;
		(*i)++;
	}
}

static void	handle_quotes(const char *input, int *i, char *quote_char)
{
	if ((input[*i] == '\'' || input[*i] == '\"') && *quote_char == 0)
	{
		*quote_char = input[*i];
		(*i)++;
	}
	else if (input[*i] == *quote_char)
	{
		*quote_char = 0;
		(*i)++;
	}
}

char	*expand_string(t_ctx *ctx, const char *input)
{
	char				result[EXPAND_BUFFER_SIZE];
	t_expand_buffer		buf;
	int					i;
	char				quote_char;

	ft_bzero(result, EXPAND_BUFFER_SIZE);
	init_expand_buffer(&buf, result, EXPAND_BUFFER_SIZE);
	i = 0;
	quote_char = 0;
	while (input[i] != '\0')
	{
		if ((input[i] == '\'' || input[i] == '\"') || input[i] == quote_char)
			handle_quotes(input, &i, &quote_char);
		else if (input[i] == '$' && quote_char != '\'')
			handle_dollar_expansion(ctx, input, &i, &buf);
		else
			handle_regular_char(input, &i, &buf);
	}
	*buf.cursor = '\0';
	return (safe_strdup(ctx, buf.result));
}
