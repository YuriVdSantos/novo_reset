
// #include "minishell.h"

// #define EXPAND_BUFFER_SIZE 4096

// // Função auxiliar para obter o valor de uma variável de ambiente
// static char	*get_var_value(t_ctx *ctx, const char *var_name, int *len)
// {
// 	char	*val;
// 	char	*key;

// 	if (var_name[0] == '?')
// 	{
// 		*len = 1;
// 		return (safe_itoa(ctx, ctx->exit_status));
// 	}
// 	if (var_name[0] == '$')
// 	{
// 		*len = 1;
// 		char *pid_str;
// 		get_pid_var(ctx, &pid_str);
// 		return (pid_str);
// 	}
// 	*len = 0;
// 	while (ft_isalnum(var_name[*len]) || var_name[*len] == '_')
// 		(*len)++;
// 	if (*len == 0)
// 	{
// 		return (safe_strdup(ctx, "$"));
// 	}

// 	// CORRECTED LINE: Change safe_strndup to ft_safe_strndup
// 	key = ft_safe_strndup(ctx, var_name, *len);
// 	if (!key)
// 		return (NULL);

// 	val = get_env_value(ctx, key);

// 	if (val)
// 		return (safe_strdup(ctx, val));
// 	return (safe_strdup(ctx, ""));
// }

// char	*expand_string(t_ctx *ctx, const char *input)
// {
// 	char	result[EXPAND_BUFFER_SIZE];
// 	char	*cursor;
// 	int		len;
// 	char	quote_char;

// 	ft_bzero(result, EXPAND_BUFFER_SIZE);
// 	cursor = result;
// 	quote_char = 0;
// 	while (*input)
// 	{
// 		if (*input == '\'' && quote_char == 0)
// 			quote_char = *input++;
// 		else if (*input == '\"' && quote_char == 0)
// 			quote_char = *input++;
// 		else if (quote_char && *input == quote_char)
// 		{
// 			quote_char = 0;
// 			input++;
// 		}		else if (*input == '$' && quote_char != '\'')
// 		{
// 			char *var_value = get_var_value(ctx, input + 1, &len);
// 			ft_strlcpy(cursor, var_value, EXPAND_BUFFER_SIZE - (cursor - result));
// 			cursor += ft_strlen(var_value);
// 			input += len + 1;
// 			free(var_value);
// 		}
// 		else
// 			*cursor++ = *input++;
// 	}
// 	*cursor = '\0';
// 	return (ft_strdup(result));
// }

// int	is_valid_dollar(char c)
// {
// 	return (ft_isalnum(c) || c == '{' || c == '?' || c == '$' || c == '_');
// }

// #include "minishell.h"

// #define EXPAND_BUFFER_SIZE 4096

// // Função auxiliar para obter o valor de uma variável de ambiente
// static char	*get_var_value(t_ctx *ctx, const char *var_name, int *len)
// {
// 	char	*val;
// 	char	*key;

// 	if (var_name[0] == '?')
// 	{
// 		*len = 1;
// 		return (safe_itoa(ctx, ctx->exit_status));
// 	}
// 	if (var_name[0] == '$')
// 	{
// 		*len = 1;
// 		char *pid_str;
// 		get_pid_var(ctx, &pid_str);
// 		return (pid_str);
// 	}
// 	*len = 0;
// 	while (ft_isalnum(var_name[*len]) || var_name[*len] == '_')
// 		(*len)++;
// 	if (*len == 0)
// 	{
// 		// If no valid variable name follows '$', return "$"
// 		return (safe_strdup(ctx, "$"));
// 	}

// 	key = ft_safe_strndup(ctx, var_name, *len);
// 	if (!key)
// 		return (NULL);

// 	val = get_env_value(ctx, key);
//     // Free the key after use, as it's a temporary allocation
//     free(key); // Add this line
// 	if (val)
// 		return (safe_strdup(ctx, val));
// 	return (safe_strdup(ctx, "")); // Return an empty string for non-existent variables
// }

// char	*expand_string(t_ctx *ctx, const char *input)
// {
// 	char	result[EXPAND_BUFFER_SIZE];
// 	char	*cursor;
// 	int		len;
// 	char	quote_char;

// 	ft_bzero(result, EXPAND_BUFFER_SIZE);
// 	cursor = result;
// 	quote_char = 0;
// 	while (*input)
// 	{
// 		if (*input == '\'' && quote_char == 0)
// 			quote_char = *input++;
// 		else if (*input == '\"' && quote_char == 0)
// 			quote_char = *input++;
// 		else if (quote_char && *input == quote_char)
// 		{
// 			quote_char = 0;
// 			input++;
// 		}
//         else if (*input == '$' && quote_char != '\'')
// 		{
// 			char *var_value = get_var_value(ctx, input + 1, &len);
//             if (!var_value) // Handle allocation failure from get_var_value
//             {
//                 ctx->exit_status = 1; // Set an error exit status
//                 return (NULL);
//             }
// 			ft_strlcpy(cursor, var_value, EXPAND_BUFFER_SIZE - (cursor - result));
// 			cursor += ft_strlen(var_value);
// 			input += len + 1;
// 			free(var_value); // Free the safely duplicated string from get_var_value
// 		}
// 		else
// 			*cursor++ = *input++;
// 	}
// 	*cursor = '\0';
// 	return (ft_strdup(result));
// }

// int	is_valid_dollar(char c)
// {
// 	return (ft_isalnum(c) || c == '{' || c == '?' || c == '$' || c == '_');
// }

#include "minishell.h"

#define EXPAND_BUFFER_SIZE 4096

// Função auxiliar para obter o valor de uma variável de ambiente
char	*get_var_value(t_ctx *ctx, const char *var_name, int *len)
{
	char	*val;
	char	*key;

    // 1. Caso especial: $?
	if (var_name[0] == '?')
	{
		*len = 1;
		return (safe_itoa(ctx, ctx->exit_status));
	}
    // 2. Caso especial: $$ (o caractere APÓS o '$' é outro '$')
	if (var_name[0] == '$')
	{
		*len = 1; // Consome o segundo '$'
		char *pid_str;
		get_pid_var(ctx, &pid_str);
		return (pid_str);
	}

    // 3. Caso especial: '$' literal.
    // Isso acontece se a string 'var_name' for vazia (ex: input original era "echo $")
    // ou se o primeiro caractere após '$' não é um caractere válido para nome de variável.
    if (var_name[0] == '\0' || (!ft_isalpha(var_name[0]) && var_name[0] != '_'))
    {
        *len = 0; // Se var_name é vazio, não consome caracteres do nome da variável.
        if (var_name[0] != '\0') // Se não é vazio, consome 1 caractere (o inválido)
            *len = 1;
        return (safe_strdup(ctx, "$")); // Retorna '$' literal como resultado da expansão.
    }

    // 4. Caso geral: Nome de variável normal
	*len = 0; // Reseta len para o comprimento do nome da variável
	while (ft_isalnum(var_name[*len]) || var_name[*len] == '_')
		(*len)++;

	key = ft_safe_strndup(ctx, var_name, *len);
	if (!key)
		return (NULL);

	val = get_env_value(ctx, key); // get_env_value retorna ponteiro direto para a lista env

	if (val)
		return (safe_strdup(ctx, val)); // Retorna uma cópia segura do valor
	return (safe_strdup(ctx, "")); // Retorna string vazia para variáveis não existentes
}

char	*expand_string(t_ctx *ctx, const char *input)
{
	char	result[EXPAND_BUFFER_SIZE];
	char	*cursor;
	int		len;
	char	quote_char;
    char    *var_value;

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
		{
			quote_char = 0;
			input++;
		}
        else if (*input == '$' && quote_char != '\'')
		{
			var_value = get_var_value(ctx, input + 1, &len);
            if (!var_value)
            {
                ctx->exit_status = 1;
                return (NULL);
            }

            // Calculate current used length in the buffer
            size_t current_filled_len = (size_t)(cursor - result); // Garante que a diferença é unsigned
            // Calculate remaining capacity for ft_strlcpy (size includes null terminator)
            size_t remaining_capacity = EXPAND_BUFFER_SIZE - current_filled_len;

            // Copia a string expandida para o buffer
            // ft_strlcpy copia min(strlen(src), size - 1) caracteres e garante null-termination
            ft_strlcpy(cursor, var_value, remaining_capacity);

            // Avança o cursor pela quantidade de caracteres realmente copiados
            // ft_strlen(cursor) aqui retorna o comprimento da string que foi colada
            cursor += ft_strlen(cursor);

			input += len + 1; // Avança o input após a variável
		}
		else
		{
			*cursor++ = *input++;
            if (cursor >= result + EXPAND_BUFFER_SIZE - 1) // Prevenir buffer overflow
                break;
		}
	}
	*cursor = '\0';
	return (safe_strdup(ctx, result));
}

int	is_valid_dollar(char c)
{
	return (ft_isalnum(c) || c == '{' || c == '?' || c == '$' || c == '_');
}