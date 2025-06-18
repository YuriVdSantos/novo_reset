
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
			env_value = get_var_value(ctx, input + 1, &len);
            if (!env_value) {
                return (NULL);
            }
			result = safe_strjoin(ctx, result, env_value);
			input += len + 1;
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

char	*expand_env_var(t_ctx *ctx, const char *input, int *len)
{
    return (get_var_value(ctx, input, len));
}

static int	ft_isalpha_upper(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	else
		return (0);
}