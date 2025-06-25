#include "minishell.h"

static const char	*get_token_name(t_token *token)
{
	if (!token || token->type == END)
		return ("newline");
	if (token->type == PIPE)
		return ("|");
	if (token->type == REDIR_IN)
		return ("<");
	if (token->type == REDIR_OUT)
		return (">");
	if (token->type == HEREDOC)
		return ("<<");
	if (token->type == APPEND)
		return (">>");
	if (token->value)
		return (token->value);
	return ("unknown");
}

static bool	is_redirection(t_token_type type)
{
	return (type >= REDIR_IN && type <= APPEND);
}

bool	validate_syntax(t_ctx *ctx, t_token *tokens)
{
	t_token		*current;
	t_token		*prev;
	char		error_msg[128];
	const char	*unexpected_token_str;

	current = tokens;
	prev = NULL;
	if (current->type == PIPE)
	{
		print_error(ctx, "syntax error near unexpected token `|'", 0, 258);
		return (false);
	}
	while (current && current->type != END)
	{
		if (prev && prev->type == PIPE && current->type == PIPE)
		{
			print_error(ctx, "syntax error near unexpected token `|'", 0, 258);
			return (false);
		}
		if (is_redirection(current->type))
		{
			if (!current->next || current->next->type != WORD)
			{
				unexpected_token_str = get_token_name(current->next);
				ft_strlcpy(error_msg, "syntax error near unexpected token `", sizeof(error_msg));
				ft_strlcat(error_msg, unexpected_token_str, sizeof(error_msg));
				ft_strlcat(error_msg, "'", sizeof(error_msg));
				print_error(ctx, error_msg, 0, 258);
				return (false);
			}
		}
		
		if (current->type == PIPE && (!current->next || current->next->type == END))
		{
			print_error(ctx, "syntax error near unexpected token `|'", 0, 258);
			return (false);
		}
		prev = current;
		current = current->next;
	}
	return (true);
}
