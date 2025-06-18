/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:17:26 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/17 17:06:55 by yurivieirad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_pid_var(t_ctx *ctx, char **str) // Add t_ctx *ctx parameter
{
	char	*pid_str_tmp;

	pid_str_tmp = ft_itoa(getpid()); // Allocates memory
	if (!pid_str_tmp)
	{
		*str = NULL; // Indicate allocation failure
		return ;
	}
	*str = safe_strdup(ctx, pid_str_tmp); // Use safe_strdup for proper allocation tracking
	free(pid_str_tmp); // Free the temporary string from ft_itoa
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

	// CORRECTED LINE: Change safe_strndup to ft_safe_strndup
	*str = ft_safe_strndup(ctx, start, len);
	*input += len;
	return (len);
}

int	define_substring(t_ctx *ctx, char **str, const char **input, t_token_type type) // Add t_ctx *ctx parameter
{
	if (type == SQUOTE)
		return (substr_handle_squote(str, input));
	else if (type == DQUOTE)
		return (substr_handle_dquote(str, input));
	else if (type == ENV_VAR)
		return (substr_handle_env_var(ctx, str, input)); // Pass ctx to substr_handle_env_var
	return (0);
}