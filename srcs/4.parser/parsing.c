/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:14:35 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/15 20:46:52 by yurivieirad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_variables(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	if (only_var_assignments(ctx->token_list))
	{
		if ((*tmp)->type == ENV_VAR)
			handle_env_var(ctx, tmp, current);
		else if ((*tmp)->type == ASSIGNMENT_VAR)
			handle_assignment_var(ctx, tmp, current);
		return ;
	}
	else if ((*tmp)->type == ENV_VAR)
	{
		handle_env_var(ctx, tmp, current);
		return ;
	}
	else
		handle_word(tmp, current);
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
			handle_word(&tmp, current);
		else if (tmp->type == DQUOTE)
			handle_dquote(ctx, &tmp, current);
		else if (tmp->type == SQUOTE)
			handle_squote(ctx, &tmp, current);
		else if (tmp->type == ENV_VAR || tmp->type == ASSIGNMENT_VAR)
			handle_variables(ctx, &tmp, current);
		else
			return (handle_error(ctx, "Parsing error", -1, 2), NULL);
	}
	return (head);
}

int	only_var_assignments(t_token *tokens)
{
	t_token	*current;

	current = *tokens;
	while (current && current->type != END)
	{
		if (current->type != ASSIGNMENT_VAR && current->type != ENV_VAR)
		{
			set_type_word(tokens);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

void	set_type_word(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	if (current->type == ASSIGNMENT_VAR || current->type == ENV_VAR)
	{
		current->type = PASS;
		if (!current->next)
			return ;
		current = current->next;
	}
	while (current)
	{
		if (current->type == ASSIGNMENT_VAR || current->type == ENV_VAR)
			current->type = WORD;
		current = current->next;
	}
}
