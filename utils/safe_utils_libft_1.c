/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_utils_libft_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:48:45 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/01 14:14:49 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*safe_strrchr(t_ctx *ctx, const char *s, int c)
{
	char	*ptr;

	ptr = ft_strrchr(s, c);
	if (ptr == NULL)
		return (NULL);
	return (safe_strdup(ctx, ptr));
}

char	*safe_strnstr(t_ctx *ctx, const char *haystack, \
	const char *needle, size_t len)
{
	char	*ptr;

	ptr = ft_strnstr(haystack, needle, len);
	if (ptr == NULL)
		return (NULL);
	return (safe_strdup(ctx, ptr));
}

char	*safe_strtrim(t_ctx *ctx, char const *s1, char const *set)
{
	char	*trimmed;

	trimmed = ft_strtrim(s1, set);
	if (trimmed == NULL)
		return (NULL);
	return (safe_strdup(ctx, trimmed));
}

char	*safe_strjoin(t_ctx *ctx, char const *s1, char const *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	if (joined == NULL)
		return (NULL);
	register_alloc(ctx, joined, ALLOC_TYPE_STRING);
	return (joined);
}

char	*safe_itoa(t_ctx *ctx, long n)
{
	char	*tmp;
	char	*result;

	tmp = ft_itoa(n);
	if (!tmp)
		return (NULL);
	result = safe_strdup(ctx, tmp);
	free(tmp);
	return (result);
}
