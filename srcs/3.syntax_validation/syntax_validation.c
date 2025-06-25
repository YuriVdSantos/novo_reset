
// #include "minishell.h"

// static bool	check_redir_position(t_ctx *ctx, t_token *tokens);
// static bool	check_empty_commands(t_ctx *ctx, t_token *tokens);
// static bool	check_consecutive_ops(t_ctx *ctx, t_token *tokens);
// static bool	check_heredoc_delimiters(t_ctx *ctx, t_token *tokens);


// bool	validate_syntax(t_ctx *ctx, t_token *tokens)
// {
// 	if (!check_pipes(ctx, tokens) ||
// 		!check_redirections(ctx, tokens) ||
// 		!check_consecutive_ops(ctx, tokens) ||
// 		!check_heredoc_delimiters(ctx, tokens) ||
// 		!check_redir_position(ctx, tokens) ||
// 		!check_empty_commands(ctx, tokens))
// 	{
// 		return (false);
// 	}
// 	return (true);
// }

// static bool	check_redir_position(t_ctx *ctx, t_token *tokens)
// {
// 	t_token	*current;
// 	t_token	*previous;

// 	current = tokens;
// 	previous = NULL;
// 	while (current)
// 	{
// 		if (current->type >= REDIR_IN && current->type <= APPEND)
// 		{
// 			if (previous == NULL || previous->type == PIPE)
// 			{
// 				if (!current->next || current->next->type != WORD)
// 					return (syntax_error(ctx, "syntax error near redirect"));
// 			}
// 		}
// 		previous = current;
// 		current = current->next;
// 	}
// 	return (true);
// }

// static bool	check_empty_commands(t_ctx *ctx, t_token *tokens)
// {
// 	t_token	*current;
// 	t_token	*previous;

// 	current = tokens;
// 	previous = NULL;
// 	while (current)
// 	{
// 		if (current->type == PIPE)
// 		{
// 			if (previous == NULL)
// 				return (syntax_error(ctx, "syntax error near pipe"));
// 			if (previous->type == PIPE)
// 				return (syntax_error(ctx, "syntax error near pipe"));
// 			if (previous->type >= REDIR_IN && previous->type <= APPEND)
// 				return (syntax_error(ctx, "syntax error near pipe"));
// 			if (!current->next || current->next->type == END)
// 				return (syntax_error(ctx, "syntax error near pipe"));
// 		}
// 		else if (current->type >= REDIR_IN && current->type <= APPEND)
// 			if (!current->next || current->next->type == END)
// 				return (syntax_error(ctx, "syntax error near redirect"));
// 		previous = current;
// 		current = current->next;
// 	}
// 	return (true);
// }

// static bool	check_consecutive_ops(t_ctx *ctx, t_token *tokens)
// {
// 	t_token	*current;
// 	t_token	*previous;

// 	current = tokens;
// 	previous = NULL;
// 	while (current)
// 	{
// 		if (previous)
// 		{
// 			if ((previous->type >= REDIR_IN && previous->type <= APPEND) &&
// 				(current->type >= REDIR_IN && current->type <= APPEND))
// 			{
// 				return (syntax_error(ctx,
// 					"syntax error: consecutive redirects"));
// 			}
// 		}
// 		previous = current;
// 		current = current->next;
// 	}
// 	return (true);
// }


// static bool	check_heredoc_delimiters(t_ctx *ctx, t_token *tokens)
// {
// 	t_token	*current;
// 	char	*delim;

// 	current = tokens;
// 	while (current)
// 	{
// 		if (current->type == HEREDOC)
// 		{
// 			if (!current->next || current->next->type != WORD)
// 				return (syntax_error(ctx, "syntax error: heredoc delimiter"));
// 			delim = current->next->value;
// 			while (*delim)
// 			{
// 				if (ft_strchr("$\\`;|&<>", *delim))
// 					return (syntax_error(ctx, "invalid heredoc delimiter"));
// 				delim++;
// 			}
// 		}
// 		current = current->next;
// 	}
// 	return (true);
// }

#include "minishell.h"

// Função auxiliar para verificar se um token é um operador de redirecionamento ou pipe
static bool	is_operator(t_token_type type)
{
	return (type == PIPE || type == REDIR_IN || type == REDIR_OUT
		|| type == HEREDOC || type == APPEND);
}

// Função auxiliar para verificar se o token é um redirecionamento
static bool is_redirection(t_token_type type)
{
    return (type >= REDIR_IN && type <= APPEND);
}


bool	validate_syntax(t_ctx *ctx, t_token *tokens)
{
	t_token	*current;
	t_token	*prev;

	current = tokens;
	prev = NULL;
	if (current->type == PIPE)
	{
		print_error(ctx, "syntax error near unexpected token `|'", 0, 258);
		return (false);
	}
	while (current && current->type != END)
	{
		// Regra 1: Dois operadores seguidos são um erro (ex: `||`, `>> >`, `| <`)
		if (prev && is_operator(prev->type) && is_operator(current->type))
		{
			print_error(ctx, "syntax error near unexpected token", 0, 258);
			return (false);
		}
		// Regra 2: Redirecionamento deve ser seguido por um WORD
		if (is_redirection(current->type))
		{
			if (!current->next || current->next->type != WORD)
			{
				print_error(ctx, "syntax error near unexpected token `newline'", 0, 258);
				return (false);
			}
		}
        // Regra 3: Um pipe não pode ser o último token
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