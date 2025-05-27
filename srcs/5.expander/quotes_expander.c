/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:15:27 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/26 03:47:50 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_dquotes(t_ctx *ctx, const char *input)
{
	char	*result;
	char	*ptr;

	result = NULL;
	ptr = result;
	while (*input)
	{
		if (*input == '$' && (input[1] == '"' || input[1] == '\''))
			input++;
		else if (*input == '$' && (ft_isalnum(input[1]) || input[1] == '{'))
			ptr = expand_env_var(ctx, input + 1);
		else
			*ptr++ = *input++;
	}
	*ptr = '\0';
	return (result);
}
