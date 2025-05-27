/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:14:13 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/25 23:39:19 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_handle_pipe(t_ctx *ctx, const char **input, t_token **tokens)
{
	t_token_type	type;
	char			c;

	c = **input;
	if (c == '|')
		type = PIPE;
	(*input)++;
	ft_lstadd_back(tokens, new_token(ctx, type, NULL));
}
