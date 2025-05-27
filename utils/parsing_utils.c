/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:26:04 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/26 01:33:34 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd(t_ctx *ctx)
{
	t_cmd	*cmd;

	cmd = safe_malloc(ctx, sizeof(t_cmd), ALLOC_TYPE_CMD);
	cmd->args = safe_malloc(ctx, sizeof(char *), ALLOC_TYPE_STR);
	cmd->args[0] = NULL;
	cmd->redirections = NULL;
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
	new_args = safe_malloc(ctx, sizeof(char *) * (count + 2), \
	ALLOC_TYPE_STR);
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

void	add_redir(t_ctx *ctx, t_cmd *cmd, t_redir_type type, char *file)
{
	t_redir	*new_redir;
	t_redir	*last;

	new_redir = safe_malloc(ctx, sizeof(t_redir), ALLOC_TYPE_REDIR);
	new_redir->type = type;
	new_redir->filename = safe_strdup(ctx, file);
	new_redir->next = NULL;
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
