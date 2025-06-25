/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:14:35 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/05 20:10:45 by yurivieirad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_handle_redir(t_ctx *ctx, const char **input, t_token **tokens)
{
	t_token_type	type;
	char			c;

	type = ERROR;
	c = **input;
	(*input)++;
	if (c == '>' && **input == '|')
	{
		type = REDIR_OUT;
		(*input)++;
	}
	else if (**input == c && c == '<')
		type = HEREDOC;
	else if (**input == c && c == '>')
		type = APPEND;
	else
	{
		if (c == '<')
			type = REDIR_IN;
		else if (c == '>')
			type = REDIR_OUT;
		(*input)--;
	}
	(*input)++;
	ft_lstadd_back(tokens, new_token(ctx, type, NULL));
}
