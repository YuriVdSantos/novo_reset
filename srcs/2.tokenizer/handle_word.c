/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:18:26 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/02 19:38:56 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	quote_chr(const char *start);

static int	is_metachar(char c)
{
	return (ft_isspace(c) || c == '|' || c == '<' || c == '>');
}

void	token_handle_word(t_ctx *ctx, const char **input, t_token **tokens)
{
	const char	*start;
	int			len;
	char		*word;

	start = *input;
	len = 0;
	while (start[len] && !is_metachar(start[len]))
	{
		if (start[len] == '\'' || start[len] == '\"')
			len += quote_chr(start + len);
		else
			len++;
	}
	if (len > 0)
	{
		word = ft_safe_strndup(ctx, start, len);
		ft_lstadd_back(tokens, new_token(ctx, WORD, word));
	}
	*input += len;
}

static char	quote_chr(const char *start)
{
	char	quote_char;
	int		i;

	if (*start != '\'' && *start != '\"')
		return (0);
	quote_char = *start;
	i = 1;
	while (start[i] && start[i] != quote_char)
		i++;
	if (start[i] == quote_char)
		i++;
	return (i);
}
