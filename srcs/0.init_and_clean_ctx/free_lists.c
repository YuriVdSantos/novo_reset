/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_lists.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:15:54 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/02 18:48:28 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir_list(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->filename);
		redir->filename = NULL;
		free(redir);
		redir = next;
	}
	redir = NULL;
}

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*current_cmd;
	t_cmd	*next_cmd;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		next_cmd = current_cmd->next;
		if (current_cmd->args)
		{
			free(current_cmd->args);
		}
		if (current_cmd->cmd_path)
			free(current_cmd->cmd_path);
		free_redir_list(current_cmd->redirections);
		free(current_cmd);
		current_cmd = next_cmd;
	}
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
