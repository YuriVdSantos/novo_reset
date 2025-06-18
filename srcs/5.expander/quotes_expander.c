// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   quotes_expander.c                                  :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/05/25 18:15:27 by jhualves          #+#    #+#             */
// /*   Updated: 2025/06/17 17:07:19 by yurivieirad      ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// static int	ft_isalpha_upper(int c);

// char	*expand_dquotes(t_ctx *ctx, const char *input)
// {
// 	char	*result;
// 	char	*env_value;
// 	int		len;

// 	result = ft_strdup("");
// 	while (*input)
// 	{
// 		if (*input == '$')
// 		{
// 			input++;
// 			len = var_name_length(input);
// 			env_value = expand_env_var(ctx, input, &len);
// 			result = safe_strjoin(ctx, result, env_value);
// 			input += len;
// 			// free(env_value);
// 		}
// 		else
// 			result = safe_strjoin(ctx, result, (char []){*input++, '\0'});
// 	}
// 	if (result == NULL)
// 	{
// 		handle_error(ctx, "Memory allocation error in expand_dquotes", 12, 1);
// 		return (NULL);
// 	}	
// 	return (result);
// }

// char	*get_env_value(t_ctx *ctx, const char *key)
// {
// 	t_env	*node;

// 	// If the key is NULL or an empty string, it's not a valid lookup.
// 	// Special handling for '?' and '$$' is now done in get_var_value.
// 	if (!key || ft_strcmp(key, "") == 0)
// 		return (NULL);

// 	node = find_env_var(ctx->env_list, key);
// 	if (node && node->value)
// 		return (node->value); // Return direct pointer to the environment variable's value
// 	return (NULL); // Key not found in environment
// }

// int	var_name_length(const char *input)
// {
// 	int	len;

// 	len = 0;
// 	while (input[len] && (ft_isalpha_upper(input[len]) || \
// 	ft_isdigit(input[len]) || input[len] == '_'))
// 		len++;
// 	return (len);
// }

// char	*expand_env_var(t_ctx *ctx, const char *input, int *len)
// {
// 	char	*key;

// 	key = ft_substr(input, 0, *len);
// 	if (!key)
// 	{
// 		handle_error(ctx, "Memory allocation error in expand_env_var", 12, 1);
// 		return (NULL);
// 	}
// 	return (get_env_value(ctx, key));
// }

// static int	ft_isalpha_upper(int c)
// {
// 	if (c >= 'A' && c <= 'Z')
// 		return (1);
// 	else
// 		return (0);
// }


// File: novo_reset_minishell/srcs/5.expander/quotes_expander.c

#include "minishell.h"

static int	ft_isalpha_upper(int c);

char	*expand_dquotes(t_ctx *ctx, const char *input)
{
	char	*result;
	char	*env_value;
	int		len;

    result = safe_strdup(ctx, "");
    if (!result) return (NULL);

	while (*input)
	{
		if (*input == '$')
		{
            // CORRIGIDO: Passar 'input + 1' para get_var_value
			env_value = get_var_value(ctx, input + 1, &len);
            if (!env_value) {
                return (NULL);
            }
			result = safe_strjoin(ctx, result, env_value);
			input += len + 1; // Avança o input por 'len' caracteres do nome da variável + 1 para o '$'
		}
		else
			result = safe_strjoin(ctx, result, (char []){*input++, '\0'});
	}
	if (result == NULL)
	{
		handle_error(ctx, "Memory allocation error in expand_dquotes", 12, 1);
		return (NULL);
	}
	return (result);
}

char	*get_env_value(t_ctx *ctx, const char *key)
{
	t_env	*node;

	if (!key || ft_strcmp(key, "") == 0)
		return (NULL);

	node = find_env_var(ctx->env_list, key);
	if (node)
		return (node->value);
	return (NULL);
}

int	var_name_length(const char *input)
{
	int	len;

	len = 0;
	if (input[0] == '?')
		return (1);
	if (input[0] == '$')
		return (1);
	while (input[len] && (ft_isalpha_upper(input[len]) || \
	ft_isdigit(input[len]) || input[len] == '_'))
		len++;
	return (len);
}

// Esta função agora deve ser simplesmente um wrapper para get_var_value,
// ou pode ser removida se não for mais usada em nenhum outro lugar.
char	*expand_env_var(t_ctx *ctx, const char *input, int *len)
{
    // A função get_var_value é a responsável por expandir a variável.
    // expand_env_var pode ser um wrapper ou ser removida.
    // Se for mantida, deve chamar get_var_value.
    return (get_var_value(ctx, input, len));
}

static int	ft_isalpha_upper(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	else
		return (0);
}