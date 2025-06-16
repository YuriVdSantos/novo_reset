// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   ft_echo.c                                          :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/06/09 20:51:03 by jhualves          #+#    #+#             */
// /*   Updated: 2025/06/10 00:27:13 by yurivieirad      ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// int	is_only_n(const char *str)
// {
// 	int	j;

// 	j = 2;
// 	while (str[j])
// 	{
// 		if (str[j] != 'n')
// 			return (0);
// 		j++;
// 	}
// 	return (1);
// }

// int	ft_echo(char **args, t_ctx *ctx)
// {
// 	int	i;
// 	int	new_line;
	
// 	//printf("Exit Status: %d\n", ctx->exit_status);
// 	//printf("Executing echo with args: %s\n", args[1]);
// 	new_line = 1;
// 	i = 1;
// 	while (args[i] && ft_strncmp(args[i], "-n", 2) == 0 && is_only_n(args[i]))
// 	{
// 		new_line = 0;
// 		i++;
// 	}
// 	while (args[i])
// 	{
// 		//printf("Executing echo with args: %s\n", args[i]);
// 		//Se Args[i] for igual a '?', imprimir o exit status
// 		if (args[i][0] == '?' && args[i][1] == '\0')
// 		{
// 			ft_putnbr_fd(ctx->exit_status, STDOUT_FILENO);
// 			if (args[i + 1])
// 				ft_putstr_fd(" ", STDOUT_FILENO);
// 			i++;
// 			continue;
// 		}
// /* 		if (args[i] == '?')
// 		{
// 			ft_putnbr_fd(ctx->exit_status, STDOUT_FILENO);
// 			i++;
// 			continue;
// 		} */
// 		ft_putstr_fd(args[i], STDOUT_FILENO);
// 		if (args[i + 1])
// 			ft_putstr_fd(" ", STDOUT_FILENO);
// 		i++;
// 	}
// 	if (new_line)
// 		ft_putstr_fd("\n", STDOUT_FILENO);

// 	return (0);
// }

#include "minishell.h"
#include <stdbool.h>

static bool	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (false);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (i > 1);
}

int	ft_echo(char **args)
{
	int		i;
	bool	newline;

	i = 1;
	newline = true;

	while (args[i] && is_n_flag(args[i]))
	{
		newline = false;
		i++;
	}

	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}

	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
