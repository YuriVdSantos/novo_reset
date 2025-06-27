/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 21:10:42 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/26 21:20:25 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir_type	define_type(t_token **tmp);

void	handle_redir(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	t_redir_type	type;
	char			*expanded_filename;
	char			*final_filename;

	type = define_type(tmp);
	*tmp = (*tmp)->next;
	if (!*tmp || (*tmp)->type != WORD)
		return (print_error(ctx, "syntax error near unexpected token", -1, 2));
	expanded_filename = (*tmp)->value;
	if (!expanded_filename)
	{
		ctx->exit_status = 1;
		return ;
	}
	final_filename = remove_quotes(expanded_filename);
	if (!final_filename)
	{
		ctx->exit_status = 1;
		return ;
	}
	add_redir(current, type, final_filename);
	free(final_filename);
	*tmp = (*tmp)->next;
}

static t_redir_type	define_type(t_token **tmp)
{
	t_redir_type	type;

	if ((*tmp)->type == REDIR_IN)
		type = REDIR_INPUT;
	else if ((*tmp)->type == REDIR_OUT)
		type = REDIR_OUTPUT;
	else if ((*tmp)->type == HEREDOC)
		type = REDIR_HEREDOC;
	else
		type = REDIR_APPEND;
	return (type);
}
