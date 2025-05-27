/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:45:49 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/26 03:58:49 by jhualves         ###   ########.fr       */
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
	t_env	*env_node;

	env_node = find_env_var(ctx->env_list, key);
	if (env_node)
		return (env_node->value);
	else if (ft_strcmp(key, "?") == 0)
		return (ft_itoa(ctx->exit_status));
	else if (ft_strcmp(key, "$") == 0)
		return (ft_itoa(getpid()));
	return (ft_strdup(""));
}

char	*expand_env_var(t_ctx *ctx, const char *input)
{
	char	var_name[256];
	char	*value;
	int		len;

	len = 0;
	if (input[0] == '{') // Caso ${VAR}
	{
		input++;
		while (input[len] && input[len] != '}')
			len++;
		ft_strlcpy(var_name, input, len + 1);
		input += len + 1; // Avança o '}'
	}
	else // Caso $VAR ou $1 etc
	{
		while (input[len] && (ft_isalnum(input[len]) || input[len] == '_'))
			len++;
		ft_strlcpy(var_name, input, len + 1);
		input += len;
	}
	value = get_env_value(ctx, var_name);
	return (value);
}

char	*expand_string(t_ctx *ctx, const char *input)
{
	char	*result;
	char	*var_value;
	char	*buffer;

	result = safe_malloc(ctx, BUFFER_SIZE, ALLOC_TYPE_STRING);
	buffer = result;
	while (*input)
	{
		if (*input == '$' && (ft_isalnum(input[1]) || \
		input[1] == '{' || input[1] == '?' || input[1] == '$'))
		{
			var_value = expand_env_var(ctx, input + 1);
			ft_strlcpy(buffer, var_value, BUFFER_SIZE - (buffer - result));
			buffer += ft_strlen(var_value);
			input += var_name_length(input + 1) + 1; // Função auxiliar para calcular tamanho
			safe_free(ctx, var_value);
		}
		else
			*buffer++ = *input++;
	}
	*buffer = '\0';
	return (result);
}

int	var_name_length(const char *input)
{
	int	len;

	len = 0;
	if (*input == '{')
	{
		input++;
		while (input[len] && input[len] != '}')
			len++;
		return (len + 2);
	}
	else
	{
		while (input[len] && (ft_isalnum(input[len]) || input[len] == '_'))
			len++;
		return (len + 1);
	}
}
