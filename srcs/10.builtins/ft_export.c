

// #include "minishell.h"

// static int	is_valid_identifier(const char *name)
// {
// 	if (!name || (!ft_isalpha(*name) && *name != '_'))
// 		return (0);
// 	name++;
// 	while (*name)
// 	{
// 		if (!ft_isalnum(*name) && *name != '_')
// 			return (0);
// 		name++;
// 	}
// 	return (1);
// }

// static int	print_export_env(t_env *env_list)
// {
// 	t_env	*current;

// 	current = env_list;
// 	while (current)
// 	{
// 		ft_putstr_fd("declare -x ", STDOUT_FILENO);
// 		ft_putstr_fd(current->key, STDOUT_FILENO);
// 		if (current->value != NULL)
// 		{
// 			ft_putstr_fd("=\"", STDOUT_FILENO);
// 			ft_putstr_fd(current->value, STDOUT_FILENO);
// 			ft_putstr_fd("\"", STDOUT_FILENO);
// 		}
// 		ft_putstr_fd("\n", STDOUT_FILENO);
// 		current = current->next;
// 	}
// 	return (EXIT_SUCCESS);
// }

// static char	*get_key_from_assignment(const char *assignment)
// {
// 	const char *equal_pos;

// 	equal_pos = ft_strchr(assignment, '=');
// 	if (!equal_pos)
// 		return (ft_strdup(assignment));
// 	return (ft_strndup(assignment, equal_pos - assignment));
// }

// int	ft_export(char **args, t_ctx *ctx)
// {
// 	int		exit_status;
// 	int		i;
// 	char	*key;

// 	exit_status = EXIT_SUCCESS;
// 	i = 1;
// 	if (!args[1])
// 		return (print_export_env(ctx->env_list));
// 	while (args[i])
// 	{
// 		key = get_key_from_assignment(args[i]);
// 		if (!is_valid_identifier(key))
// 		{
// 			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
// 			ft_putstr_fd(args[i], STDERR_FILENO);
// 			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
// 			exit_status = EXIT_FAILURE;
// 		}
// 		else if (ft_strchr(args[i], '='))
// 		{
// 			set_env_var(ctx, args[i]);
// 		}
// 		else if (!find_env_var(ctx->env_list, key))
// 		{
// 			add_new_env_var(ctx, ft_strdup(key), NULL, args[i]);
// 		}
// 		free(key);
// 		i++;
// 	}
// 	return (exit_status);
// }

/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* ft_export.c                                        :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/06/15 22:15:12 by jhualves         ###   ########.fr       */
/* Updated: 2025/06/15 22:15:12 by jhualves         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

// Imprime as variáveis de ambiente no formato `declare -x KEY="VALUE"`.
static int	print_export_env(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	// NOTA: Para ser 100% como o bash, a lista deveria ser ordenada alfabeticamente.
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

// Extrai a chave de uma atribuição (ex: "KEY=value" -> "KEY").
static char	*get_key_from_assignment(const char *assignment)
{
	const char	*equal_pos;

	equal_pos = ft_strchr(assignment, '=');
	if (!equal_pos)
		return (ft_strdup(assignment));
	return (ft_strndup(assignment, equal_pos - assignment));
}

// Lógica principal do `export`.
int	ft_export(char **args, t_ctx *ctx)
{
	int		exit_status;
	int		i;
	char	*key;

	exit_status = EXIT_SUCCESS;
	i = 1;
	if (!args[1])
		return (print_export_env(ctx->env_list));
	while (args[i])
	{
		key = get_key_from_assignment(args[i]);
		if (!is_valid_env_identifier(key))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = EXIT_FAILURE;
		}
		else if (ft_strchr(args[i], '='))
			set_env_var(ctx, args[i]);
		else if (!find_env_var(ctx->env_list, key))
		{
			// Exporta uma variável sem valor.
			add_new_env_var(ctx, ft_strdup(key), NULL, args[i]);
		}
		free(key);
		i++;
	}
	return (exit_status);
}
