/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_lists.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:15:54 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/04 22:20:37 by jhualves         ###   ########.fr       */
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

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		free(cmd->args);
		cmd->args = NULL;
		free(cmd->cmd_path);
		cmd->cmd_path = NULL;
		free_redir_list(cmd->redirections);
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
