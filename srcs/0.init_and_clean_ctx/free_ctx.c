/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ctx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 19:22:03 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/26 20:18:17 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_context(t_ctx *ctx)
{
	if (ctx)
	{
		super_free(ctx);
		if (ctx->input)
			free(ctx->input);
		if (ctx->pwd)
			free(ctx->pwd);
		if (ctx->oldpwd)
			free(ctx->oldpwd);
		if (ctx->env_list)
			free_env_list(ctx->env_list);
		if (ctx->env_list_str)
			free_string_array(ctx->env_list_str);
		ctx->pwd = NULL;
		ctx->oldpwd = NULL;
		ctx->input = NULL;
		ctx->last_error_message = NULL;
		ctx->env_list = NULL;
		free(ctx);
	}
	ctx = NULL;
}

void	free_all_allocations(t_ctx *ctx)
{
	t_allocation	*alloc;
	t_allocation	*next;

	alloc = ctx->allocations;
	while (alloc)
	{
		next = alloc->next;
		if (alloc->ptr)
		{
			if (alloc->type == ALLOC_TYPE_STR)
				free_string_array(alloc->ptr);
			else
			{
				if (alloc->ptr)
					free(alloc->ptr);
				alloc->ptr = NULL;
			}
		}
		free(alloc);
		alloc = next;
	}
	ctx->allocations = NULL;
}

void	super_free(t_ctx *ctx)
{
	t_cmd		*cmd_list;

	cmd_list = ctx->cmd_list;
	free_cmd_list(cmd_list);
	ctx->cmd_list = NULL;
	free_token_list(ctx->token_list);
	ctx->token_list = NULL;
	free_all_allocations(ctx);
}

void	safe_free(t_ctx *ctx, void *ptr)
{
	t_allocation	*current;
	t_allocation	*prev;

	if (!ptr || !ctx || !ctx->allocations)
		return ;
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
