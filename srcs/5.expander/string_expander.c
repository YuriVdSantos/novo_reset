#include "minishell.h"

#define EXPAND_BUFFER_SIZE 4096

char	*get_var_value(t_ctx *ctx, const char *var_name, int *len)
{
	char	*val;
	char	*key;

	if (var_name[0] == '?')
	{
		*len = 1;
		return (safe_itoa(ctx, ctx->exit_status));
	}
	if (var_name[0] == '$')
	{
		*len = 1;
		char *pid_str;
		get_pid_var(ctx, &pid_str);
		return (pid_str);
	}
	if (var_name[0] == '\0' || (!ft_isalpha(var_name[0]) && var_name[0] != '_'))
	{
		*len = 0;
		return (safe_strdup(ctx, "$"));
	}
	*len = 0;
	while (ft_isalnum(var_name[*len]) || var_name[*len] == '_')
		(*len)++;
	key = ft_safe_strndup(ctx, var_name, *len);
	if (!key)
		return (NULL);
	val = get_env_value(ctx, key);
	if (val)
		return (safe_strdup(ctx, val));
	return (safe_strdup(ctx, ""));
}

char	*expand_string(t_ctx *ctx, const char *input)
{
	char	result[EXPAND_BUFFER_SIZE];
	char	*cursor;
	int		i;
	char	quote_char;

	ft_bzero(result, EXPAND_BUFFER_SIZE);
	cursor = result;
	i = 0;
	quote_char = 0;
	while (input[i])
	{
		if (quote_char == 0 && (input[i] == '\'' || input[i] == '\"'))
		{
			quote_char = input[i];
			i++;
			continue ;
		}
		if (quote_char != 0 && input[i] == quote_char)
		{
			quote_char = 0;
			i++;
			continue ;
		}
		if (input[i] == '$' && quote_char != '\'')
		{
			int len;
			char *var_value = get_var_value(ctx, &input[i + 1], &len);
			if (var_value)
			{
				if ((cursor - result) + ft_strlen(var_value) < EXPAND_BUFFER_SIZE)
				{
					ft_strlcpy(cursor, var_value, EXPAND_BUFFER_SIZE - (cursor - result));
					cursor += ft_strlen(var_value);
				}
				// Liberar a memória alocada por get_var_value
				// Se get_var_value usar safe_malloc, a memória será liberada no final.
				// Caso contrário, você precisará de um free aqui.
			}
			i += len + 1;
		}
		else
		{
			if (cursor - result < EXPAND_BUFFER_SIZE - 1)
				*cursor++ = input[i];
			i++;
		}
	}
	*cursor = '\0';
	return (safe_strdup(ctx, result));
}

int	is_valid_dollar(char c)
{
	return (ft_isalnum(c) || c == '{' || c == '?' || c == '$' || c == '_');
}