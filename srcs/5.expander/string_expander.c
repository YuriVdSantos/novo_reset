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

	// 1. Inicialização das variáveis
	ft_bzero(result, EXPAND_BUFFER_SIZE);
	cursor = result;
	i = 0;
	quote_char = 0;

	// 2. Loop principal para percorrer a string de entrada
	while (input[i] != '\0')
	{
		// Caso A: Lida com a entrada ou saída de um estado de aspas
		if ((input[i] == '\'' || input[i] == '\"') && quote_char == 0)
		{
			quote_char = input[i]; // Entra no modo de aspas
			i++;
		}
		else if (input[i] == quote_char && quote_char != 0)
		{
			quote_char = 0; // Sai do modo de aspas
			i++;
		}
		// Caso B: Lida com a expansão de variáveis ($)
		else if (input[i] == '$' && quote_char != '\'')
		{
			int len;
			char *var_value;

			len = 0;
			var_value = get_var_value(ctx, &input[i + 1], &len);
			if (var_value)
			{
				// Copia o valor da variável para o resultado, se houver espaço
				if ((cursor - result) + ft_strlen(var_value) < EXPAND_BUFFER_SIZE)
				{
					ft_strlcpy(cursor, var_value, EXPAND_BUFFER_SIZE - (cursor - result));
					cursor += ft_strlen(var_value);
				}
			}
			// Avança o índice para depois do nome da variável (ex: pula "$USER")
			i += len + 1;
		}
		// Caso C: Lida com todos os outros caracteres (copia literal)
		else
		{
			// Copia o caractere atual para o resultado, se houver espaço
			if (cursor - result < EXPAND_BUFFER_SIZE - 1)
			{
				*cursor = input[i];
				cursor++;
			}
			i++;
		}
	}

	// 3. Finaliza a string de resultado e a retorna
	*cursor = '\0';
	return (safe_strdup(ctx, result));
}

int	is_valid_dollar(char c)
{
	return (ft_isalnum(c) || c == '{' || c == '?' || c == '$' || c == '_');
}