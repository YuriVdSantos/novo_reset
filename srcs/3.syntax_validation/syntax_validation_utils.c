/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:27:25 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/26 20:56:07 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_pipes(t_ctx *ctx, t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (current->type == PIPE)
		return (syntax_error(ctx, "syntax error near unexpected `|'"));
	while (current)
	{
		if (current->type == PIPE)
		{
			if (!current->next || current->next->type == PIPE || \
				current->next->type == END)
				return (syntax_error(ctx, \
					"syntax error near unexpected `|'"));
		}
		current = current->next;
	}
	return (true);
}

bool	is_valid_filename_token(t_token_type type)
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

bool	check_initial_pipe(t_ctx *ctx, t_token *tokens)
{
	if (tokens->type == PIPE)
	{
		print_error(ctx, "syntax error near unexpected token `|'", 0, 258);
		return (false);
	}
	return (true);
}

bool	check_consecutive_pipes(t_ctx *ctx, t_token *prev, t_token *current)
{
	if (prev && prev->type == PIPE && current->type == PIPE)
	{
		print_error(ctx, "syntax error near unexpected token `|'", 0, 258);
		return (false);
	}
	return (true);
}
