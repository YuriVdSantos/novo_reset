/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:44:22 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/02 20:12:52 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	if (!str)
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

static int	is_within_long_long_range(const char *str)
{
	int			len;
	const char	*limit;
	int			is_negative;

	is_negative = 0;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			is_negative = 1;
		str++;
	}
	len = ft_strlen(str);
	if (is_negative)
		limit = "9223372036854775808";
	else
		limit = "9223372036854775807";
	if (len > (int)ft_strlen(limit))
		return (0);
	if (len == (int)ft_strlen(limit) && ft_strcmp(str, limit) > 0)
		return (0);
	return (1);
}

int	ft_exit(char **args, t_ctx *ctx)
{
	long long	status;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args[1])
	{
		status = ctx->exit_status;
		free_context(ctx);
		close_all_fds();
		exit(status);
	}
	if (!is_numeric(args[1]) || !is_within_long_long_range(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		free_context(ctx);
		close_all_fds();
		exit(2);
	}
	else if (args[2])
		return (print_error(ctx, "minishell: exit: too many arguments", 2, 2), \
			1);
	else
		status = ft_atodbl(args[1]);
	return (free_context(ctx), exit((unsigned char)status), 0);
}
