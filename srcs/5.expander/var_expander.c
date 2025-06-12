/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:45:49 by jhualves          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/06/10 19:59:38 by yurivieirad      ###   ########.fr       */
=======
/*   Updated: 2025/06/10 23:16:50 by jhualves         ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define BUFFER_SIZE 2025
// if (var[0] == '?') {
//     char *exit_code = ft_itoa(ctx->current_exit_status);
//     expanded_var = safe_strdup(ctx, exit_code);
//     free(exit_code);
// }
char	*get_env_value(t_ctx *ctx, const char *key)
{
	t_env	*node;

	if (!key)
		return ("");
	if (ft_strcmp(key, "?") == 0)
		return (ft_itoa(ctx->exit_status));
	if (ft_strcmp(key, "$") == 0)
		return (ft_itoa(getpid()));
	node = find_env_var(ctx->env_list, key);
	if (node && node->value)
		return (node->value);
	return ("");
}

char	*expand_env_var(t_ctx *ctx, const char *input, int *len)
{
	char	name[256];
	char	*raw;
	char	*copy;
	int		i;

	i = 0;
	if (input[0] == '{')
	{
		input++;
		while (input[i] && input[i] != '}' && i < 255)
			name[i++] = input[i];
		name[i] = '\0';
		*len = i + 2;
	}
	else if (input[0] == '?' || input[0] == '$')
	{
		name[0] = input[0];
		name[1] = '\0';
		*len = 1;
	}
	else
	{
		while ((ft_isalnum(input[i]) || input[i] == '_') && i < 255)
			name[i++] = input[i];
		name[i] = '\0';
		*len = i;
	}
	raw = get_env_value(ctx, name);
	copy = safe_strdup(ctx, raw);
	if (name[0] == '?' || name[0] == '$')
		free(raw);
	return (copy);
}

int	is_valid_dollar(char c)
{
	return (ft_isalnum(c) || c == '{' || c == '?' || c == '$' || c == '_');
}
