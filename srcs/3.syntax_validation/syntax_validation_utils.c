/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:27:25 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/17 17:51:21 by yurivieirad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_pipes(t_ctx *ctx, t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (current->type == PIPE)
		return (syntax_error(ctx, "syntax error near unexpected token `|'"));
	while (current)
	{
		if (current->type == PIPE)
		{
			if (!current->next || current->next->type == PIPE || \
				current->next->type == END)
				return (syntax_error(ctx, \
					"syntax error near unexpected token `|'"));
		}
		current = current->next;
	}
	return (true);
}

static bool	is_valid_filename_token(t_token_type type)
{
	return (type == WORD || type == SQUOTE || type == DQUOTE);
}

bool	check_redirections(t_ctx *ctx, t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type >= REDIR_IN && current->type <= APPEND)
		{
			if (!current->next || !is_valid_filename_token(current->next->type))
				return (syntax_error(ctx, "syntax error near redirection"));
		}
		current = current->next;
	}
	return (true);
}
