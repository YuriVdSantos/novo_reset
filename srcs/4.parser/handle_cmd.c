/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 21:10:42 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/27 21:23:01 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe(t_token **tmp, t_cmd **current)
{
	t_cmd	*new;

	new = new_cmd();
	(*current)->next = new;
	*current = new;
	*tmp = (*tmp)->next;
}

void	handle_word(t_token **tmp, t_cmd *current)
{
	add_arg(current, (*tmp)->value);
	*tmp = (*tmp)->next;
}

void	handle_dquote(t_ctx *ctx, t_token **tmp, t_cmd **current)
{
	char	*content;

	content = expand_dquotes(ctx, (*tmp)->value);
	add_arg(*current, content);
	free(content);
	*tmp = (*tmp)->next;
}

void	handle_squote(t_ctx *ctx, t_token **tmp, t_cmd **current)
{
	char	*content;

	content = safe_strdup(ctx, (*tmp)->value);
	add_arg(*current, content);
	*tmp = (*tmp)->next;
}

void	set_type_word(t_token *tokens)
{
	(void)tokens;
}
