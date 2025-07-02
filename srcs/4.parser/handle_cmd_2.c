/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 21:10:42 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/02 19:43:49 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir_type	define_type(t_token **tmp);

void	handle_redir(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	t_redir_type	type;
	char			*filename_or_delimiter;

	type = define_type(tmp);
	*tmp = (*tmp)->next;
	if (!*tmp || ((*tmp)->type != WORD && \
			(*tmp)->type != SQUOTE && (*tmp)->type != DQUOTE))
	{
		print_error(ctx, "syntax error near unexpected token", -1, 2);
		return ;
	}
	if (type == REDIR_HEREDOC)
		filename_or_delimiter = ft_strdup((*tmp)->value);
	else
		filename_or_delimiter = remove_quotes((*tmp)->value);
	if (!filename_or_delimiter)
	{
		ctx->exit_status = 1;
		return ;
	}
	add_redir(current, type, filename_or_delimiter);
	free(filename_or_delimiter);
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
