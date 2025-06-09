/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:12:24 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/05 20:05:00 by yurivieirad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	closed_quotation(t_ctx *ctx, const char **input, char c);

void	token_handle_quote(t_ctx *ctx, const char **input, t_token **tokens)
{
	t_token_type	type;
	char			*str;
	char			c;

	type = ERROR;
	str = NULL;
	c = **input;
	(*input)++;
	if (!closed_quotation(ctx, input, c))
		type = ERROR;
	else if (**input != c && c == '"')
	{
		type = DQUOTE;
		define_substring(&str, input, type);
	}
	else if (**input != c && c == '\'')
	{
		type = SQUOTE;
		define_substring(&str, input, type);
	}
	else if (**input == c)
	{
		str = ft_strdup("");
		(*input)++;
	}
	ft_lstadd_back(tokens, new_token(ctx, type, str));
}

static int	closed_quotation(t_ctx *ctx, const char **input, char c)
{
	int		i;

	i = 1;
	while ((*input)[i] && (*input)[i] != '\0' && (*input)[i] != '\n')
	{
		if ((*input)[i] == c)
			return (1);
		i++;
	}
	syntax_error(ctx, "Syntax_error: Unclosed quotation");
	return (0);
}
