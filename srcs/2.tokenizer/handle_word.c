
#include "minishell.h"

static int	is_metachar(char c)
{
	return (ft_isspace(c) || c == '|' || c == '<' || c == '>');
}

void	token_handle_word(t_ctx *ctx, const char **input, t_token **tokens)
{
	const char	*start;
	int			len;
	char		quote_char;

	start = *input;
	len = 0;
	while (start[len] && !is_metachar(start[len]))
	{
		if (start[len] == '\'' || start[len] == '\"')
		{
			quote_char = start[len];
			len++;
			while (start[len] && start[len] != quote_char)
				len++;
			if (start[len] == quote_char)
				len++;
		}
		else
		{
			len++;
		}
	}
	if (len > 0)
	{
		char *word = ft_safe_strndup(ctx, start, len);
		ft_lstadd_back(tokens, new_token(ctx, WORD, word));
	}
	*input += len;
}
