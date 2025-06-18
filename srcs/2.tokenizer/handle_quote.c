
#include "minishell.h"

static int	closed_quotation(t_ctx *ctx, const char *input, char c)
{
	const char	*start;

	start = input;
	while (*start && *start != c)
		start++;
	if (*start == c)
		return (1);
    // Se a citação não for fechada, imprima o erro e retorne 0.
    // O token de erro será adicionado posteriormente.
	syntax_error(ctx, "Syntax_error: Unclosed quotation");
	return (0);
}

void	token_handle_quote(t_ctx *ctx, const char **input, t_token **tokens)
{
	t_token_type	type;
	char			*str;
	const char		*start;
	int				len;
	char			quote_char;

	quote_char = **input;
	(*input)++; // Move past the opening quote

    // Primeiro, verifica se a citação está fechada.
	if (!closed_quotation(ctx, *input, quote_char))
	{
        // Se a citação não for fechada, adiciona um token de ERRO.
        // O tokenizer continuará lendo até o final da linha para evitar mais erros.
		ft_lstadd_back(tokens, new_token(ctx, ERROR, NULL));
		while (**input) // Consome o resto do input para evitar loop infinito em caso de erro
			(*input)++;
		return ;
	}
    // Se a citação está fechada, extrai o conteúdo entre aspas.
	start = *input;
	len = 0;
	while (start[len] && start[len] != quote_char)
		len++;
    // str será o conteúdo da citação, SEM aspas
	str = ft_safe_strndup(ctx, start, len); // str é alocado e rastreado

	if (quote_char == '\'')
		type = SQUOTE;
	else
		type = DQUOTE;
	ft_lstadd_back(tokens, new_token(ctx, type, str));
	*input += len + 1; // Move past the content and the closing quote
}
