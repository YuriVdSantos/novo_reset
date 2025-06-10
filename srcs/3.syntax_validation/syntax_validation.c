/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:27:25 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/09 22:46:49 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_redir_position(t_ctx *ctx, t_token *tokens);
static bool	check_empty_commands(t_ctx *ctx, t_token *tokens);
static bool	check_consecutive_ops(t_ctx *ctx, t_token *tokens);
static bool	check_heredoc_delimiters(t_ctx *ctx, t_token *tokens);
static bool	check_pipes(t_ctx *ctx, t_token *tokens);
static bool	check_redirections(t_ctx *ctx, t_token *tokens);

bool	validate_syntax(t_ctx *ctx, t_token *tokens)
{
	if (!check_pipes(ctx, tokens) ||
		!check_redirections(ctx, tokens) ||
		!check_consecutive_ops(ctx, tokens) ||
		!check_heredoc_delimiters(ctx, tokens) ||
		!check_redir_position(ctx, tokens) ||
		!check_empty_commands(ctx, tokens))
	{
		return (false);
	}
	return (true);
}

// static bool	check_pipes(t_ctx *ctx, t_token *tokens)
// {
// 	t_token	*current;

// 	current = tokens;
// 	if (current->type == PIPE)
// 		return (syntax_error(ctx, "syntax error near unexpected token `|'"));
// 	while (current && current->type != END)
// 	{
// 		if (current->type == PIPE)
// 		{
// 			if (!current->next || current->next->type == PIPE)
// 				return (syntax_error(ctx, "syntax error near `|'"));
// 		}
// 		current = current->next;
// 	}
// 	return (true);
// }

// static bool	check_redirections(t_ctx *ctx, t_token *tokens)
// {
// 	t_token	*current;

// 	current = tokens;
// 	while (current && current->type != END)
// 	{
// 		if (current->type >= REDIR_IN && current->type <= APPEND)
// 		{
// 			if (!current->next || current->next->type != WORD)
// 				return (syntax_error(ctx, "syntax error near redirection"));
// 		}
// 		current = current->next;
// 	}
// 	return (true);
// }


static bool	check_redir_position(t_ctx *ctx, t_token *tokens)
{
	t_token	*current;
	t_token	*previous;

	current = tokens;
	previous = NULL;
	while (current)
	{
		if (current->type >= REDIR_IN && current->type <= APPEND)
		{
			if (previous == NULL || previous->type == PIPE)
			{
				if (!current->next || current->next->type != WORD)
					return (syntax_error(ctx, "syntax error near redirect"));
			}
		}
		previous = current;
		current = current->next;
	}
	return (true);
}

static bool	check_empty_commands(t_ctx *ctx, t_token *tokens)
{
	t_token	*current;
	t_token	*previous;

	current = tokens;
	previous = NULL;
	while (current)
	{
		if (current->type == PIPE)
		{
			if (previous == NULL)
				return (syntax_error(ctx, "syntax error near pipe"));
			if (previous->type == PIPE)
				return (syntax_error(ctx, "syntax error near pipe"));
			if (previous->type >= REDIR_IN && previous->type <= APPEND)
				return (syntax_error(ctx, "syntax error near pipe"));
			if (!current->next || current->next->type == END)
				return (syntax_error(ctx, "syntax error near pipe"));
		}
		else if (current->type >= REDIR_IN && current->type <= APPEND)
			if (!current->next || current->next->type == END)
				return (syntax_error(ctx, "syntax error near redirect"));
		previous = current;
		current = current->next;
	}
	return (true);
}

static bool	check_consecutive_ops(t_ctx *ctx, t_token *tokens)
{
	t_token	*current;
	t_token	*previous;

	current = tokens;
	previous = NULL;
	while (current)
	{
		if (previous)
		{
			if ((previous->type >= REDIR_IN && previous->type <= APPEND) && \
				(current->type >= REDIR_IN && current->type <= APPEND))
			{
				return (syntax_error(ctx, \
					"syntax error: consecutive redirects"));
			}
			if (previous->type == PIPE \
				&& (current->type >= REDIR_IN && current->type <= APPEND))
			{
				return (syntax_error(ctx, "syntax error near redirect"));
			}
		}
		previous = current;
		current = current->next;
	}
	return (true);
}


static bool	check_heredoc_delimiters(t_ctx *ctx, t_token *tokens)
{
	t_token	*current;
	char	*delim;

	current = tokens;
	while (current)
	{
		if (current->type == HEREDOC)
		{
			if (!current->next || current->next->type != WORD)
				return (syntax_error(ctx, "syntax error: heredoc delimiter"));
			delim = current->next->value;
			while (*delim)
			{
				if (ft_strchr("$\\`;|&<>", *delim))
					return (syntax_error(ctx, "invalid heredoc delimiter"));
				delim++;
			}
		}
		current = current->next;
	}
	return (true);
}

static bool	check_pipes(t_ctx *ctx, t_token *tokens)
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

static bool	check_redirections(t_ctx *ctx, t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
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
