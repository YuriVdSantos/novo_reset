/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:34:30 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/10 23:01:17 by yurivieirad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	token_error_check(t_ctx *ctx, t_token **head);

t_token	*tokenize_input(t_ctx *ctx, const char **input)
{
	t_token	*tokens;

	tokens = NULL;
	while (*input && **input && **input != '\0' && **input != '\n')
	{
		if (ft_isspace(**input))
			(*input)++;
		else if (**input == '|')
			token_handle_pipe(ctx, input, &tokens);
		else if (**input == '<' || **input == '>')
			token_handle_redir(ctx, input, &tokens);
		else if (**input == '\'' || **input == '\"')
			token_handle_quote(ctx, input, &tokens);
		else if (**input == '$')
			token_handle_env_var(ctx, input, &tokens);
		else
			token_handle_word(ctx, input, &tokens);
		if (!token_error_check(ctx, &tokens))
			return (free_token_list(tokens), NULL);
	}
	ft_lstadd_back(&tokens, new_token(
		ctx, END, "NULL"));
	return (tokens);
}

void	ft_lstadd_back(t_token **head, t_token *new_node)
{
	t_token	*current;

	if (!new_node)
		return ;
	if (!head || !new_node)
		return ;
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next != NULL)
		current = current->next;
	current->next = new_node;
}

t_token	*new_token(t_ctx *ctx, t_token_type type, const char *str)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		print_error(ctx, "Memory allocation failed for token", -1, 2);
		return (NULL);
	}
	token->type = type;
	if (!str)
		token->value = NULL;
	else
		token->value = ft_strdup(str);
	token->next = NULL;
	return (token);
}

static int	token_error_check(t_ctx *ctx, t_token **head)
{
	t_token	*current;

	if (!head)
		return (0);
	current = *head;
	while (current)
	{
		if (current->type == ERROR)
		{
			print_error(ctx, "Tokenization error", -1, 2);
			return (0);
		}
		current = current->next;
	}
	return (1);
}
