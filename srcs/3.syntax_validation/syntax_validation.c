/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 20:39:00 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/02 19:44:23 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static bool	is_redirection(t_token_type type)
{
	return (type >= REDIR_IN && type <= APPEND);
}

bool	check_redirection_syntax(t_ctx *ctx, t_token *current)
{
	char		error_msg[128];
	const char	*unexpected_token_str;

	if (is_redirection(current->type))
	{
		if ((!current->next || current->next->type != WORD)
			&& (current->next->type != SQUOTE) && \
				(current->next->type != DQUOTE))
		{
			unexpected_token_str = get_token_name(current->next);
			ft_strlcpy(error_msg, "syntax error near unexpected token `", \
				sizeof(error_msg));
			ft_strlcat(error_msg, unexpected_token_str, sizeof(error_msg));
			ft_strlcat(error_msg, "'", sizeof(error_msg));
			print_error(ctx, error_msg, 0, 258);
			return (false);
		}
	}
	return (true);
}

bool	check_final_pipe(t_ctx *ctx, t_token *current)
{
	if (current->type == PIPE && (!current->next || current->next->type == END))
	{
		print_error(ctx, "syntax error near unexpected token `|'", 0, 258);
		return (false);
	}
	return (true);
}
