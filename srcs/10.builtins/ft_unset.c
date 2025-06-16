/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 19:56:56 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/15 22:58:59 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*find_env_var_value(t_env *env_list, const char *value);

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

int	ft_unset(char **args, t_ctx *ctx)
{
	int		exit_status;
	int		i;
	t_env	*current;

	current = find_env_var_value(ctx->env_list, args[1]);
	exit_status = EXIT_SUCCESS;
	i = 1;
	while (args[i])
	{
		if (!current)
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = EXIT_FAILURE;
		}
		else
		{
			unset_string_env_var(ctx, current->key);
			unset_env_var(ctx, current->key);
		}	
		i++;
	}
	return (exit_status);
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
			current->key = NULL;
			free(current->value);
			current->value = NULL;
			free(current);
			current = NULL;
			break ;
		}
		prev = current;
		current = current->next;
	}
}

static t_env	*find_env_var_value(t_env *env_list, const char *value)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->value, value) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}
