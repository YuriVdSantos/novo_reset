/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ctx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:15:54 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/27 01:13:24 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_context(t_ctx *ctx)
{
	if (ctx)
	{
		free_all_allocations(ctx);
		free(ctx->pwd);
		free(ctx->oldpwd);
		free(ctx->input);
		free(ctx->last_error_message);
		free_env_list(ctx->env_list);
		free(ctx);
	}
}

void	free_all_allocations(t_ctx *ctx)
{
	t_allocation	*alloc;
	t_allocation	*next;

	alloc = ctx->allocations;
	while (alloc)
	{
		next = alloc->next;
		if (alloc->type == ALLOC_TYPE_CMD)
			free_cmd_list(alloc->ptr);
		else if (alloc->type == ALLOC_TYPE_TOKEN)
			free_token_list(alloc->ptr);
		else if (alloc->type == ALLOC_TYPE_REDIR)
			free_redir_list(alloc->ptr);
		else if (alloc->type == ALLOC_TYPE_STR)
			free_string_array(alloc->ptr);
		else if (alloc->ptr)
			free(alloc->ptr);
		free(alloc);
		alloc = next;
	}
	ctx->allocations = NULL;
}

void	super_free(t_ctx *ctx, char **input)
{
	if (*input)
	{
		free(*input);
		input = NULL;
	}
	free_all_allocations(ctx);
}

void	safe_free(t_ctx *ctx, void *ptr)
{
	t_allocation	*current;
	t_allocation	*prev;

	current = ctx->allocations;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				ctx->allocations = current->next;
			free(current);
			free(ptr);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
