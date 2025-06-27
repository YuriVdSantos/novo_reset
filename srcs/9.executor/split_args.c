/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:34:09 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/27 15:36:34 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	replace_spaces_with_placeholder(char *str, char delimiter);
static void	restore_placeholder_to_spaces(char **exec_args);
static int	has_quote_mark(char *str);
static void	remove_all_quotes(char *str);

char	**split_args(t_ctx *ctx, char *command)
{
	char	**exec_args;

	if (!has_quote_mark(command) && ft_strchr(command, ' '))
	{
		exec_args = (char **)safe_malloc(ctx, sizeof(char *) * 2, 8);
		if (!exec_args)
			return (NULL);
		exec_args[0] = safe_strdup(ctx, command);
		if (!exec_args[0])
		{
			free(exec_args);
			return (NULL);
		}
		exec_args[1] = NULL;
		return (exec_args);
	}
	if (!has_quote_mark(command))
		return (ft_split(command, ' '));
	replace_spaces_with_placeholder(command, '"');
	replace_spaces_with_placeholder(command, '\'');
	remove_all_quotes(command);
	exec_args = ft_split(command, ' ');
	restore_placeholder_to_spaces(exec_args);
	return (exec_args);
}

static void	restore_placeholder_to_spaces(char **exec_args)
{
	char	*current_str;

	while (*exec_args)
	{
		current_str = *exec_args;
		while (*current_str)
		{
			if (*current_str == -1)
				*current_str = ' ';
			current_str++;
		}
		exec_args++;
	}
}

static void	remove_all_quotes(char *str)
{
	char	open_quote;

	open_quote = 0;
	while (*str)
	{
		if (!open_quote && (*str == '\'' || *str == '"'))
		{
			open_quote = *str;
			ft_memmove(str, str + 1, ft_strlen(str + 1) + 1);
		}
		else if (open_quote && *str == open_quote)
		{
			open_quote = 0;
			ft_memmove(str, str + 1, ft_strlen(str + 1) + 1);
		}
		else
			str++;
	}
}

static void	replace_spaces_with_placeholder(char *str, char delimiter)
{
	while (*str)
	{
		if (*str == delimiter)
		{
			str++;
			while (*str && *str != delimiter)
			{
				if (*str == ' ')
					*str = -1;
				str++;
			}
		}
		if (*str)
			str++;
	}
}

static int	has_quote_mark(char *str)
{
	while (str && *str)
	{
		if (is_quote(*str))
			return (1);
		str++;
	}
	return (0);
}
