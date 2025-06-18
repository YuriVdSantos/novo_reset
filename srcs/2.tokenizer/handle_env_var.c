/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:12:27 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/17 17:51:28 by yurivieirad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_handle_env_var(t_ctx *ctx, const char **input, t_token **tokens)
{
	t_token_type	type;
	char			*str;
	char			c;

	str = NULL;
	c = **input;
	if (c == '$')
		type = ENV_VAR;
	else
	{
		type = ERROR; // Should ideally be handled with an actual error message
		return ;
	}
	(*input)++; // Move past the '$' character
	define_substring(ctx, &str, input, type); // Pass ctx to define_substring
	ft_lstadd_back(tokens, new_token(ctx, type, str));
}
