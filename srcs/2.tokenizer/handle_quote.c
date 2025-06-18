
#include "minishell.h"

static int	closed_quotation(t_ctx *ctx, const char *input, char c)
{
	const char	*start;

	start = input;
	while (*start && *start != c)
		start++;
	if (*start == c)
		return (1);


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
	(*input)++;


	if (!closed_quotation(ctx, *input, quote_char))
	{
    
    
		ft_lstadd_back(tokens, new_token(ctx, ERROR, NULL));
		while (**input)
			(*input)++;
		return ;
	}

	start = *input;
	len = 0;
	while (start[len] && start[len] != quote_char)
		len++;

	str = ft_safe_strndup(ctx, start, len);

	if (quote_char == '\'')
		type = SQUOTE;
	else
		type = DQUOTE;
	ft_lstadd_back(tokens, new_token(ctx, type, str));
	*input += len + 1;
}
