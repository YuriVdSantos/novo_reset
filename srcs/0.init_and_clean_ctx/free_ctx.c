/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ctx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:15:54 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/26 17:01:54 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	free_context(t_ctx *ctx)
{
	if (ctx)
	{
		super_free(ctx);
		if (ctx->pwd)
			free(ctx->pwd);
		if (ctx->oldpwd)
			free(ctx->oldpwd);
		if (ctx->input)
			free(ctx->input);
		if (ctx->last_error_message)
			free(ctx->last_error_message);
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
	t_token		*token_list;

	cmd_list = ctx->cmd_list;
	token_list = ctx->token_list;
	free_all_allocations(ctx);
	free_cmd_list(cmd_list);
	free_token_list(token_list);
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
