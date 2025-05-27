/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:12:27 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/26 19:55:04 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_handle_env_var(t_ctx *ctx, const char **input, t_token **tokens)
{
	t_token_type	type;
	char			*str;

	type = ENV_VAR;
	(*input)++;
	define_substring(ctx, &str, input, type);
	ft_lstadd_back(tokens, new_token(ctx, type, str));
}
