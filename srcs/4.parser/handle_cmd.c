/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:23:43 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/26 03:38:56 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe(t_ctx *ctx, t_token **tmp, t_cmd **current)
{
	t_cmd	*new;

	new = new_cmd(ctx);
	(*current)->next = new;
	*current = new_cmd(ctx);
	*tmp = (*tmp)->next;
}

void	handle_redir(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	t_redir_type	type;

	if ((*tmp)->type == REDIR_IN)
		type = REDIR_INPUT;
	else if (((*tmp)->type == REDIR_OUT))
		type = REDIR_OUTPUT;
	else if (((*tmp)->type == HEREDOC))
		type = REDIR_HEREDOC;
	else if ((*tmp)->type == APPEND)
		type = REDIR_APPEND;
	*tmp = (*tmp)->next;
	if (!*tmp || (*tmp)->type != WORD)
	{
		print_error(ctx, \
			"syntax error near unexpected token", -1, 2);
		return ;
	}
	add_redir(ctx, current, type, (*tmp)->value);
	*tmp = (*tmp)->next;
}

void	handle_word(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	add_arg(ctx, current, (*tmp)->value);
	*tmp = (*tmp)->next;
}

void	handle_dquote(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	char	*content;

	content = safe_strtrim(ctx, (*tmp)->value, "\"");
	add_arg(ctx, current, safe_strjoin(ctx, "\"", content));
	*tmp = (*tmp)->next;
}

void	handle_squote(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	char	*content;

	content = safe_strtrim(ctx, (*tmp)->value, "'");
	add_arg(ctx, current, content);
	*tmp = (*tmp)->next;
}
