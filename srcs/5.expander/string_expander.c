// #include "minishell.h"

// #define EXPAND_BUFFER_SIZE 4096

// char	*expand_string(t_ctx *ctx, const char *input)
// {
// 	char	result[EXPAND_BUFFER_SIZE];
// 	char	*cursor;
// 	int		len;

// 	ft_bzero(result, EXPAND_BUFFER_SIZE);
// 	cursor = result;
// 	while (*input)
// 	{
// 		if (*input == '$' && is_valid_dollar(input[1]))
// 		{
// 			char *var_value = expand_env_var(ctx, input + 1, &len);
// 			ft_strlcpy(cursor, var_value, EXPAND_BUFFER_SIZE - (cursor - result));
// 			cursor += ft_strlen(var_value);
// 			input += len + 1;
// 			if (ft_strcmp(var_value, "?") == 0 || ft_strcmp(var_value, "$") == 0)
// 				free(var_value);
// 		}
// 		else
// 		{
// 			*cursor++ = *input++;
// 			if (cursor >= result + EXPAND_BUFFER_SIZE - 1)
// 				break ;
// 		}
// 	}
// 	*cursor = '\0';
// 	return (safe_strdup(ctx, result));
// }
// int	is_valid_dollar(char c)
// {
// 	return (ft_isalnum(c) || c == '{' || c == '?' || c == '$' || c == '_');
// }
#include "minishell.h"

#define EXPAND_BUFFER_SIZE 4096

// Função auxiliar para obter o valor de uma variável de ambiente
static char	*get_var_value(t_ctx *ctx, const char *var_name, int *len)
{
	char	*val;
	char	*key;

	if (var_name[0] == '?')
	{
		*len = 1;
		return (ft_itoa(ctx->exit_status));
	}
	if (var_name[0] == '$')
	{
		*len = 1;
		return (ft_itoa(getpid()));
	}
	*len = 0;
	while (ft_isalnum(var_name[*len]) || var_name[*len] == '_')
		(*len)++;
	if (*len == 0)
		return (ft_strdup("$")); // Retorna '$' literal se não for um nome válido
	key = ft_strndup(var_name, *len);
	val = get_env_value(ctx, key);
	free(key);
	if (val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}

char	*expand_string(t_ctx *ctx, const char *input)
{
	char	result[EXPAND_BUFFER_SIZE];
	char	*cursor;
	int		len;
	char	quote_char;

	ft_bzero(result, EXPAND_BUFFER_SIZE);
	cursor = result;
	quote_char = 0;
	while (*input)
	{
		if (*input == '\'' && quote_char == 0)
			quote_char = *input++;
		else if (*input == '\"' && quote_char == 0)
			quote_char = *input++;
		else if (quote_char && *input == quote_char)
			quote_char = *input++ == 0;
		else if (*input == '$' && quote_char != '\'')
		{
			char *var_value = get_var_value(ctx, input + 1, &len);
			ft_strlcpy(cursor, var_value, EXPAND_BUFFER_SIZE - (cursor - result));
			cursor += ft_strlen(var_value);
			input += len + 1;
			free(var_value);
		}
		else
			*cursor++ = *input++;
	}
	*cursor = '\0';
	return (ft_strdup(result));
}

int	is_valid_dollar(char c)
{
	return (ft_isalnum(c) || c == '{' || c == '?' || c == '$' || c == '_');
}
