/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:50:51 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/27 15:51:39 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_env_identifier(const char *name)
{
	if (!name || (!ft_isalpha(*name) && *name != '_'))
		return (0);
	name++;
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

void	unset_env_var(t_ctx *ctx, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = ctx->env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				ctx->env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(char **args, t_ctx *ctx)
{
	int		exit_status;
	int		i;
	bool	changed;

	exit_status = EXIT_SUCCESS;
	changed = false;
	i = 0;
	while (args[++i])
	{
		if (!is_valid_env_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = EXIT_FAILURE;
		}
		else
		{
			if (find_env_var(ctx->env_list, args[i]))
				changed = true;
			unset_env_var(ctx, args[i]);
		}
	}
	if (changed)
		sync_env_list_str(ctx);
	return (exit_status);
}
