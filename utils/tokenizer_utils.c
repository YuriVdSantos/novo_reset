/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:17:26 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/26 22:17:47 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_pid_var(t_ctx *ctx, char **str)
{
	char	*pid_str_tmp;

	pid_str_tmp = ft_itoa(getpid());
	if (!pid_str_tmp)
	{
		*str = NULL;
		return ;
	}
	*str = safe_strdup(ctx, pid_str_tmp);
	free(pid_str_tmp);
}

static int	substr_handle_squote(char **str, const char **input)
{
	const char	*start;
	int			len;

	start = (const char *)*input;
	len = 0;
	while (start[len] != '\0' && start[len] != '\'')
		len++;
	*str = ft_strndup(start, len);
	*input += len + 1;
	return (len);
}

static int	substr_handle_dquote(char **str, const char **input)
{
	const char	*start;
	int			len;

	start = (const char *)*input;
	len = 0;
	while (start[len] && start[len] != '\"')
		len++;
	*str = ft_strndup(start, len);
	*input += len + 1;
	return (len);
}

static int	substr_handle_env_var(t_ctx *ctx, char **str, const char **input)
{
	const char	*start;
	int			len;

	if ((*input)[0] == '$')
	{
		get_pid_var(ctx, str);
		*input += 2;
		return (2);
	}
	start = (const char *)*input;
	len = 0;
	while (start[len] && !ft_isspace(start[len]) && start[len] != '$' \
			&& start[len] != '\'' && start[len] != '\"')
		len++;
	*str = ft_safe_strndup(ctx, start, len);
	*input += len;
	return (len);
}

int	define_substring(t_ctx *ctx, char **str, const char **input, \
	t_token_type type)
{
	if (type == SQUOTE)
		return (substr_handle_squote(str, input));
	else if (type == DQUOTE)
		return (substr_handle_dquote(str, input));
	else if (type == ENV_VAR)
		return (substr_handle_env_var(ctx, str, input));
	return (0);
}
