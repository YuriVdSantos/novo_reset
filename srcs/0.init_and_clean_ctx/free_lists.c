/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_lists.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:15:54 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/13 17:45:42 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir_list(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		if (redir->filename)
			free(redir->filename);
		redir->filename = NULL;
		next = redir->next;
		free(redir);
		redir = next;
	}
	redir = NULL;
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		free_string_array(cmd->args);
		cmd->args = NULL;
		if (cmd->cmd_path)
		{
			free(cmd->cmd_path);
			cmd->cmd_path = NULL;
		}
		if (cmd->redirections)
			free_redir_list(cmd->redirections);
		next = cmd->next;
		free(cmd);
		cmd = next;
	}
	cmd = NULL;
}

void	free_token_list(t_token *token)
{
	t_token	*next;

	while (token)
	{
		next = token->next;
		free(token->value);
		token->value = NULL;
		free(token);
		token = next;
	}
	token = NULL;
}

void	free_string_array(char **array)
{
	int	i;

	i = 0;
	if (array[i] == NULL)
		return ;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}
