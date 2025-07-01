/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:26:04 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/01 17:33:30 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	cmd->args = malloc(sizeof(char *));
	cmd->args[0] = NULL;
	cmd->redirections = NULL;
	cmd->cmd_path = NULL;
	cmd->type = CMD_EXTERNAL;
	cmd->next = NULL;
	return (cmd);
}

void	add_arg(t_ctx *ctx, t_cmd *cmd, char *value)
{
	char	**new_args;
	size_t	count;
	size_t	i;

	count = 0;
	while (cmd->args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	i = 0;
	while (cmd->args[i])
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = safe_strdup(ctx, value);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}

void	add_redir(t_cmd *cmd, t_redir_type type, char *file)
{
	t_redir	*new_redir;
	t_redir	*last;

	new_redir = malloc(sizeof(t_redir));
	new_redir->type = type;
	new_redir->filename = ft_strdup(file);
	new_redir->next = NULL;
	new_redir->was_heredoc = (type == REDIR_HEREDOC);
	if (!cmd->redirections)
	{
		cmd->redirections = new_redir;
	}
	else
	{
		last = cmd->redirections;
		while (last->next)
			last = last->next;
		last->next = new_redir;
	}
}
