// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   handle_word.c                                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/05/24 19:37:26 by jhualves          #+#    #+#             */
// /*   Updated: 2025/06/15 22:29:27 by yurivieirad      ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// int	is_valid_var_char(char c)
// {
// 	return (ft_isalnum(c) || c == '_' || c == '-');
// }

// static int	is_assignment_var(const char *input, int *len)
// {
// 	int	i;

// 	if (!input || !is_valid_var_char(input[0]))
// 		return (-1);
// 	i = 0;
// 	while (input[i] && input[i] != '=')
// 	{
// 		if (!is_valid_var_char(input[i]))
// 			return (0);
// 		i++;
// 	}
// 	if (input[i] != '=' || i == 0)
// 		return (0);
// 	*len = i + 1;
// 	while (input[*len] && !ft_strchr(" |<>$\'\"", input[*len]))
// 		(*len)++;
// 	return (1);
// }

// void	token_handle_word(t_ctx *ctx, const char **input, t_token **tokens)
// {
// 	int		len;
// 	int		is_assignment;
// 	char	*str;

// 	len = 0;
// 	is_assignment = is_assignment_var(*input, &len);
// 	if (is_assignment != 1)
// 	{
// 		len = 0;
// 		while ((*input)[len] && !ft_strchr(" |<>&$\'\"", (*input)[len]))
// 			len++;
// 	}
// 	if (len == 0)
// 		return ;
// 	str = ft_safe_strndup(ctx, *input, len);
// 	if (is_assignment == 1)
// 		ft_lstadd_back(tokens, new_token(ctx, ASSIGNMENT_VAR, str));
// 	else
// 		ft_lstadd_back(tokens, new_token(ctx, WORD, str));
// 	*input += len;
// }


/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* handle_word.c                                      :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/05/24 19:37:26 by jhualves          #+#    #+#             */
/* Updated: 2025/06/15 21:55:11 by jhualves         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

// Função auxiliar para verificar se o caractere é um metacaractere ou espaço.
static int	is_metachar(char c)
{
	return (ft_isspace(c) || c == '|' || c == '<' || c == '>');
}

// Função para lidar com palavras que podem conter aspas.
// Em vez de quebrar a palavra, as aspas são tratadas como parte dela.
void	token_handle_word(t_ctx *ctx, const char **input, t_token **tokens)
{
	const char	*start;
	int			len;
	char		quote_char;

	start = *input;
	len = 0;
	while (start[len] && !is_metachar(start[len]))
	{
		// Se encontrar uma aspa, avança até a aspa correspondente.
		if (start[len] == '\'' || start[len] == '\"')
		{
			quote_char = start[len];
			len++;
			while (start[len] && start[len] != quote_char)
				len++;
			if (start[len] == quote_char) // Avança para depois da aspa de fecho.
				len++;
		}
		else
		{
			len++;
		}
	}
	if (len > 0)
	{
		char *word = ft_safe_strndup(ctx, start, len);
		// A verificação de ASSIGNMENT_VAR pode ser feita aqui ou no parser.
		// Por simplicidade, vamos manter como WORD por enquanto.
		ft_lstadd_back(tokens, new_token(ctx, WORD, word));
		// free(word); // new_token faz ft_strdup, então podemos libertar este.
	}
	*input += len;
}
