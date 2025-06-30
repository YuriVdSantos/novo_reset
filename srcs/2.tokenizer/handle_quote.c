/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 20:21:22 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/26 20:21:26 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	closed_quotation(t_ctx *ctx, const char *input, char c)
{
	const char	*start;

	start = input;
	while (*start && *start != c)
		start++;
	if (*start == c)
		return (1);
	syntax_error(ctx, "Syntax_error: Unclosed quotation");
	return (0);
}

void	token_handle_quote(t_ctx *ctx, const char **input, t_token **tokens)
{
	t_token_type	type;
	char			*str;
	const char		*start;
	int				len;
	char			quote_char;

	quote_char = **input;
	(*input)++;
	if (!closed_quotation(ctx, *input, quote_char))
	{
		ft_lstadd_back(tokens, new_token(ctx, ERROR, NULL));
		return ;
	}
	start = *input;
	len = 0;
	while (start[len] && start[len] != quote_char)
		len++;
	str = ft_strndup(start, len);
	if (quote_char == '\'')
		type = SQUOTE;
	else
		type = DQUOTE;
	ft_lstadd_back(tokens, new_token(ctx, type, str));
	free(str);
	*input += len + 1;
}
