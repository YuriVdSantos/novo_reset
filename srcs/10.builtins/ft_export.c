/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:47:41 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/27 15:47:42 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_export_env(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(current->key, STDOUT_FILENO);
		if (current->value != NULL)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

static int	is_valid_export_argument(const char *arg)
{
	int	i;

	i = 0;
	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (0);
	i++;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(char **args, t_ctx *ctx)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = EXIT_SUCCESS;
	if (!args[i])
	{
		print_export_env(ctx->env_list);
		return (exit_status);
	}
	while (args[i])
	{
		if (!is_valid_export_argument(args[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = EXIT_FAILURE;
		}
		else
			set_env_var(ctx, args[i]);
		i++;
	}
	return (exit_status);
}
