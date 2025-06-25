
// #include "minishell.h"

// // Função auxiliar para verificar se um token é um operador de redirecionamento ou pipe
// static bool	is_operator(t_token_type type)
// {
// 	return (type == PIPE || type == REDIR_IN || type == REDIR_OUT
// 		|| type == HEREDOC || type == APPEND);
// }

// // Função auxiliar para verificar se o token é um redirecionamento
// static bool is_redirection(t_token_type type)
// {
//     return (type >= REDIR_IN && type <= APPEND);
// }


// bool	validate_syntax(t_ctx *ctx, t_token *tokens)
// {
// 	t_token	*current;
// 	t_token	*prev;

// 	current = tokens;
// 	prev = NULL;
// 	if (current->type == PIPE)
// 	{
// 		print_error(ctx, "syntax error near unexpected token `|'", 0, 258);
// 		return (false);
// 	}
// 	while (current && current->type != END)
// 	{
// 		// Regra 1: Dois operadores seguidos são um erro (ex: `||`, `>> >`, `| <`)
// 		if (prev && is_operator(prev->type) && is_operator(current->type))
// 		{
// 			print_error(ctx, "syntax error near unexpected token", 0, 258);
// 			return (false);
// 		}
// 		// Regra 2: Redirecionamento deve ser seguido por um WORD
// 		if (is_redirection(current->type))
// 		{
// 			if (!current->next || current->next->type != WORD)
// 			{
// 				print_error(ctx, "syntax error near unexpected token `newline'", 0, 258);
// 				return (false);
// 			}
// 		}
//         // Regra 3: Um pipe não pode ser o último token
//         if (current->type == PIPE && (!current->next || current->next->type == END))
//         {
//             print_error(ctx, "syntax error near unexpected token `|'", 0, 258);
//             return (false);
//         }
// 		prev = current;
// 		current = current->next;
// 	}
// 	return (true);
// }
#include "minishell.h"

// Função auxiliar para obter o nome do token como string
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

// Função auxiliar para verificar se o token é um redirecionamento
static bool	is_redirection(t_token_type type)
{
	return (type >= REDIR_IN && type <= APPEND);
}

// Função de validação de sintaxe totalmente corrigida
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
		// MUDANÇA 1: A regra agora é específica e só proíbe `| |`
		// A sequência `| >` agora é permitida.
		if (prev && prev->type == PIPE && current->type == PIPE)
		{
			print_error(ctx, "syntax error near unexpected token `|'", 0, 258);
			return (false);
		}
		
		// MUDANÇA 2: A mensagem de erro agora é dinâmica.
		// Se um redirecionamento não for seguido por uma palavra,
		// o erro apontará para o token correto (ex: `>` ou `|`), e não para `newline`.
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