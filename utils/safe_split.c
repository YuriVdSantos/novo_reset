/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:48:45 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/26 21:54:40 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_words(const char *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static char	*get_next_word(t_ctx *ctx, const char *s, char c, size_t *pos)
{
	size_t	start;
	size_t	end;

	while (s[*pos] == c)
		(*pos)++;
	start = *pos;
	while (s[*pos] && s[*pos] != c)
		(*pos)++;
	end = *pos;
	return (ft_safe_strndup(ctx, s + start, end - start));
}

char	**safe_split(t_ctx *ctx, const char *s, char c)
{
	char	**result;
	size_t	word_ct;
	size_t	i;
	size_t	pos;

	if (!s)
		return (NULL);
	word_ct = count_words(s, c);
	result = safe_malloc(ctx, sizeof(char *) * (word_ct + 1), ALLOC_TYPE_STR);
	if (!result)
		return (NULL);
	i = -1;
	pos = 0;
	while (i++ < word_ct)
	{
		result[i] = get_next_word(ctx, s, c, &pos);
		if (!result[i])
		{
			while (i > 0)
				free(result[--i]);
			return (free(result), NULL);
		}
	}
	result[word_ct] = NULL;
	return (result);
}

int	ft_isalpha_upper(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	else
		return (0);
}
