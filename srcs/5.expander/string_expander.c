#include "minishell.h"

#define EXPAND_BUFFER_SIZE 4096

char	*expand_string(t_ctx *ctx, const char *input)
{
	char	result[EXPAND_BUFFER_SIZE];
	char	*cursor;
	int		len;

	ft_bzero(result, EXPAND_BUFFER_SIZE);
	cursor = result;
	while (*input)
	{
		if (*input == '$' && is_valid_dollar(input[1]))
		{
			char *var_value = expand_env_var(ctx, input + 1, &len);
			ft_strlcpy(cursor, var_value, EXPAND_BUFFER_SIZE - (cursor - result));
			cursor += ft_strlen(var_value);
			input += len + 1;
			if (ft_strcmp(var_value, "?") == 0 || ft_strcmp(var_value, "$") == 0)
				free(var_value);
		}
		else
		{
			*cursor++ = *input++;
			if (cursor >= result + EXPAND_BUFFER_SIZE - 1)
				break ;
		}
	}
	*cursor = '\0';
	return (safe_strdup(ctx, result));
}
