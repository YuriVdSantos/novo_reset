/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:01:45 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/11 20:46:41 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fits_in_long_long(char *str)
{
	long long	result;
	int			digit;

	if (ft_strlen(str) > 20)
		return (0);
	if (ft_strncmp(str, "-9223372036854775808", 21) == 0)
		return (1);
	result = 0;
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		digit = *str - '0';
		if (result > (LLONG_MAX - digit) / 10)
			return (0);
		result = result * 10 + digit;
		str++;
	}
	return (1);
}

static void exit_with_error(char *command, char *msg, int error)
{
	print_error_msg(command, msg);
	close_all_fds();
	rl_clear_history();
	exit(error);
}


static void	handle_exit_errors(char **args, t_ctx *ctx)
{
	if (!args)
	{
		if (args)
			free_string_array(args);
		close_all_fds();
		free_context(ctx);
		exit(EXIT_SUCCESS);
	}
	if (args[1])
	{
		if (!fits_in_long_long(args[1]))
		{
			free_string_array(args);
			free_context(ctx);
			exit_with_error("exit", "numeric argument required", BUILTIN_MISUSE);
		}
	}
	// if (ft_strcmp(*args, "") != 0)
	// {
	// 	// free_string_array(args);
	// 	free_context(ctx);
	// 	exit_with_error("exit", "too many arguments", EXIT_FAILURE);
	// }
}

int	ft_exit(char **args, t_ctx *ctx)

{
	int	exit_status;

	rl_clear_history();
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	handle_exit_errors(args, ctx);
	close_all_fds();
	if (!args[1])
		exit_status = 0;
	else
		exit_status = ft_atodbl(args[1]);
	// free_string_array(args);
	// free_context(ctx);
	exit(exit_status);
}