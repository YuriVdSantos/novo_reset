/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_utils_libft.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:48:45 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/26 01:25:06 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*safe_strdup(t_ctx *ctx, const char *s)
{
	char	*dup;

	dup = safe_malloc(ctx, ft_strlen(s) + 1, ALLOC_TYPE_STRING);
	ft_strlcpy(dup, s, ft_strlen(s) + 1);
	return (dup);
}

char	*ft_strjoin_free(t_ctx *ctx, char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (safe_strdup(ctx, result));
}

char	*ft_safe_strndup(t_ctx *ctx, const char *s, size_t size)
{
	size_t	i;
	char	*dup;

	dup = safe_malloc(ctx, size + 1, ALLOC_TYPE_STRING);
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < size)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*safe_substr(t_ctx *ctx, char const *s, \
	unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	i = 0;
	sub = safe_malloc(ctx, len + 1, ALLOC_TYPE_STRING);
	if (!sub)
		return (NULL);
	while (i < len && s[start + i])
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

char	*safe_strchr(t_ctx *ctx, const char *s, int c)
{
	char	*ptr;

	ptr = ft_strchr(s, c);
	if (ptr == NULL)
		return (NULL);
	return (safe_strdup(ctx, ptr));
}
