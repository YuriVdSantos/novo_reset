/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:27:25 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/25 21:32:20 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_pipes(t_ctx *ctx, t_token *tokens);
static bool	check_redirections(t_ctx *ctx, t_token *tokens);
static bool	check_quotes(t_ctx *ctx, t_token *tokens);

bool	validate_syntax(t_ctx *ctx, t_token *tokens)
{
	if (!check_pipes(ctx, tokens))
		return (false);
	if (!check_redirections(ctx, tokens))
		return (false);
	if (!check_quotes(ctx, tokens))
		return (false);
	return (true);
}

static bool	check_pipes(t_ctx *ctx, t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (current->type == PIPE)
		return (syntax_error(ctx, "syntax error near unexpected token `|'"));
	while (current && current->type != END)
	{
		if (current->type == PIPE)
		{
			if (!current->next || current->next->type == PIPE)
				return (syntax_error(ctx, "syntax error near `|'"));
		}
		current = current->next;
	}
	return (true);
}

static bool	check_redirections(t_ctx *ctx, t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->type != END)
	{
		if (current->type >= REDIR_IN && current->type <= APPEND)
		{
			if (!current->next || current->next->type != WORD)
				return (syntax_error(ctx, "syntax error near redirection"));
		}
		current = current->next;
	}
	return (true);
}

static bool	check_quotes(t_ctx *ctx, t_token *tokens)
{
	t_token	*current;
	int		squote;
	int		dquote;

	current = tokens;
	squote = 0;
	dquote = 0;
	while (current && current->type != END)
	{
		if (current->type == SQUOTE)
			squote++;
		else if (current->type == DQUOTE)
			dquote++;
		current = current->next;
	}
	if (squote % 2 != 0)
		return (syntax_error(ctx, "unclosed single quote"));
	if (dquote % 2 != 0)
		return (syntax_error(ctx, "unclosed double quote"));
	return (true);
}
