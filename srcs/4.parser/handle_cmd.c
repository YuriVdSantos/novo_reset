#include "minishell.h"

void	handle_pipe(t_token **tmp, t_cmd **current)
{
	t_cmd	*new;

	new = new_cmd();
	(*current)->next = new;
	*current = new;
	*tmp = (*tmp)->next;
}

// VERSÃO CORRIGIDA DA handle_redir
void	handle_redir(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	t_redir_type	type;
	// char			*filename;
	char			*expanded_filename;
	char			*final_filename;

	// Determina o tipo de redirecionamento
	if ((*tmp)->type == REDIR_IN)
		type = REDIR_INPUT;
	else if ((*tmp)->type == REDIR_OUT)
		type = REDIR_OUTPUT;
	else if ((*tmp)->type == HEREDOC)
		type = REDIR_HEREDOC;
	else
		type = REDIR_APPEND;
	*tmp = (*tmp)->next; // Avança para o token após o operador de redirecionamento
	// Verifica se há um token de nome de arquivo válido após o redirecionamento
	if (!*tmp || ((*tmp)->type != WORD && (*tmp)->type != SQUOTE
		&& (*tmp)->type != DQUOTE))
	{
		print_error(ctx, "syntax error near unexpected token", -1, 2);
		return ;
	}
	// Expande o valor do token do nome do arquivo, se necessário
	if ((*tmp)->type == SQUOTE)
		expanded_filename = safe_strdup(ctx, (*tmp)->value);
	else if ((*tmp)->type == DQUOTE)
		expanded_filename = expand_dquotes(ctx, (*tmp)->value);
	else // WORD
		expanded_filename = expand_string(ctx, (*tmp)->value);
	if (!expanded_filename)
	{
		ctx->exit_status = 1;
		return ;
	}
	// A função remove_quotes é necessária para limpar quaisquer aspas
	// que possam ter sido parte do token WORD.
	final_filename = remove_quotes(expanded_filename);
	if (!final_filename)
	{
		ctx->exit_status = 1;
		return ;
	}
	add_redir(current, type, final_filename);
	free(final_filename); // add_redir cria sua própria cópia
	*tmp = (*tmp)->next; // Avança para o token após o nome do arquivo
}

void	handle_word(t_token **tmp, t_cmd *current)
{
	add_arg(current, (*tmp)->value);
	*tmp = (*tmp)->next;
}

void	handle_dquote(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	char	*content;

	content = safe_strdup(ctx, expand_dquotes(ctx, (*tmp)->value));
	add_arg(current, content);
	*tmp = (*tmp)->next;
}

void	handle_squote(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	char	*content;

	content = safe_strdup(ctx, (*tmp)->value);
	add_arg(current, content);
	*tmp = (*tmp)->next;
}