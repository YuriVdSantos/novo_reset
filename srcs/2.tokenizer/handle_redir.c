/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:14:35 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/25 23:38:54 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_handle_redir(t_ctx *ctx, const char **input, t_token **tokens)
{
	t_token_type	type;
	char			c;

	c = **input;
	(*input)++;
	if (**input == c && c == '<')
		type = HEREDOC;
	else if (**input == c && c == '>')
		type = APPEND;
	else
	{
		if (c == '<')
			type = REDIR_IN;
		else
			type = REDIR_OUT;
		(*input)--;
	}
	(*input)++;
	ft_lstadd_back(tokens, new_token(ctx, type, NULL));
}
