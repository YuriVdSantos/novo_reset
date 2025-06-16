// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   parsing.c                                          :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/05/25 18:14:35 by jhualves          #+#    #+#             */
// /*   Updated: 2025/06/15 22:29:51 by yurivieirad      ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// static void	handle_variables(t_ctx *ctx, t_token **tmp, t_cmd *current)
// {
// 	if (only_var_assignments(ctx->token_list))
// 	{
// 		if ((*tmp)->type == ENV_VAR)
// 			handle_env_var(ctx, tmp, current);
// 		else if ((*tmp)->type == ASSIGNMENT_VAR)
// 			handle_assignment_var(ctx, tmp, current);
// 		return ;
// 	}
// 	else if ((*tmp)->type == ENV_VAR)
// 	{
// 		handle_env_var(ctx, tmp, current);
// 		return ;
// 	}
// 	else
// 		handle_word(tmp, current);
// }

// t_cmd	*parse_tokens(t_ctx *ctx, t_token **tokens)
// {
// 	t_cmd	*head;
// 	t_cmd	*current;
// 	t_token	*tmp;

// 	head = new_cmd();
// 	current = head;
// 	tmp = *tokens;
// 	while (tmp && tmp->type != END)
// 	{
// 		if (tmp->type == PIPE)
// 			handle_pipe(&tmp, &current);
// 		else if (tmp->type >= REDIR_IN && tmp->type <= APPEND)
// 			handle_redir(ctx, &tmp, current);
// 		else if (tmp->type == WORD)
// 			handle_word(&tmp, current);
// 		else if (tmp->type == DQUOTE)
// 			handle_dquote(ctx, &tmp, current);
// 		else if (tmp->type == SQUOTE)
// 			handle_squote(ctx, &tmp, current);
// 		else if (tmp->type == ENV_VAR || tmp->type == ASSIGNMENT_VAR)
// 			handle_variables(ctx, &tmp, current);
// 		else
// 			return (handle_error(ctx, "Parsing error", -1, 2), NULL);
// 	}
// 	return (head);
// }

// int	only_var_assignments(t_token *tokens)
// {
// 	t_token	*current;

// 	current = tokens;
// 	while (current && current->type != END)
// 	{
// 		if (current->type != ASSIGNMENT_VAR && current->type != ENV_VAR)
// 		{
// 			set_type_word(tokens);
// 			return (0);
// 		}
// 		current = current->next;
// 	}
// 	return (1);
// }

// void	set_type_word(t_token *tokens)
// {
// 	t_token	*current;

// 	current = tokens;
// 	if (current->type == ASSIGNMENT_VAR || current->type == ENV_VAR)
// 	{
// 		current->type = PASS;
// 		if (!current->next)
// 			return ;
// 		current = current->next;
// 	}
// 	while (current)
// 	{
// 		if (current->type == ASSIGNMENT_VAR || current->type == ENV_VAR)
// 			current->type = WORD;
// 		current = current->next;
// 	}
// }


#include "minishell.h"

// Função para remover aspas de uma string. Retorna uma nova string alocada.
char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	quote_char;
	char	*new_str;

	if (!str)
		return (NULL);
	new_str = malloc(ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote_char = str[i];
			i++;
			while (str[i] && str[i] != quote_char)
				new_str[j++] = str[i++];
			if (str[i] == quote_char)
				i++;
		}
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}


static void	process_word_token(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	char *expanded_str;
	char *final_str;

	// 1. Expandir variáveis de ambiente (como $PWD)
	expanded_str = expand_string(ctx, (*tmp)->value);
	if (!expanded_str)
		return ;
	
	// 2. Remover aspas
	final_str = remove_quotes(expanded_str);
	free(expanded_str); // Liberar a string expandida
	if (!final_str)
		return;

	// 3. Adicionar o argumento final ao comando
	add_arg(current, final_str);
	free(final_str); // add_arg faz sua própria cópia
	*tmp = (*tmp)->next;
}

t_cmd	*parse_tokens(t_ctx *ctx, t_token **tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	t_token	*tmp;

	head = new_cmd();
	current = head;
	tmp = *tokens;
	while (tmp && tmp->type != END)
	{
		if (tmp->type == PIPE)
			handle_pipe(&tmp, &current);
		else if (tmp->type >= REDIR_IN && tmp->type <= APPEND)
			handle_redir(ctx, &tmp, current);
		else if (tmp->type == WORD)
			process_word_token(ctx, &tmp, current);
		else if (tmp->type == DQUOTE)
             handle_dquote(ctx, &tmp, current);
        else if (tmp->type == SQUOTE)
             handle_squote(ctx, &tmp, current);
        else if (tmp->type == ENV_VAR)
             handle_env_var(ctx, &tmp, current);
		else
		{
			handle_error(ctx, "Parsing error: token type not handled", -1, 2);
			tmp = tmp->next;
		}
	}
	// Se o primeiro comando estiver vazio (ex: só redireções), remove-o.
	if (head && head->args[0] == NULL && head->redirections)
	{
		// Lógica para lidar com isto, se necessário.
	}
	return (head);
}

int	only_var_assignments(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->type != END && current->type != PIPE)
	{
		if (current->type != WORD || !ft_strchr(current->value, '='))
			return (0);
		current = current->next;
	}
	return (1);
}

void	set_type_word(t_token *tokens)
{
	(void)tokens;
}
