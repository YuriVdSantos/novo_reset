/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation_.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 20:53:14 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/26 20:53:36 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	validate_syntax(t_ctx *ctx, t_token *tokens)
{
	t_token		*current;
	t_token		*prev;

	if (!check_initial_pipe(ctx, tokens))
		return (false);
	current = tokens;
	prev = NULL;
	while (current && current->type != END)
	{
		if (!check_consecutive_pipes(ctx, prev, current))
			return (false);
		if (!check_redirection_syntax(ctx, current))
			return (false);
		if (!check_final_pipe(ctx, current))
			return (false);
		prev = current;
		current = current->next;
	}
	return (true);
}
