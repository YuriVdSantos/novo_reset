/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_malloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 16:04:12 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/27 00:49:24 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*safe_malloc(t_ctx *ctx, size_t size, t_alloc_type type)
{
	void			*ptr;
	t_allocation	*new_alloc;

	ptr = malloc(size);
	if (!ptr)
	{
		ctx->last_error_message = safe_strdup(ctx, "Memory allocation failed");
		ctx->current_exit_status = 1;
		return (NULL);
	}
	new_alloc = malloc(sizeof(t_allocation));
	if (!new_alloc)
	{
		free(ptr);
		ctx->last_error_message = safe_strdup(ctx, "Memory allocation failed");
		ctx->current_exit_status = 1;
		return (NULL);
	}
	new_alloc->ptr = ptr;
	new_alloc->type = type;
	new_alloc->next = ctx->allocations;
	ctx->allocations = new_alloc;
	return (ptr);
}

void	safe_free_all(t_ctx *ctx)
{
	t_allocation	*current;
	t_allocation	*next;

	current = ctx->allocations;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	ctx->allocations = NULL;
}
