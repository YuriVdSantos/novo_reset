/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_lists.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:15:54 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/19 17:40:38 by yvieira-         ###   ########.fr       */
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
	size_t	i;

    if (!cmd)
        return;
    if (cmd->args)
    {
        for (i = 0; cmd->args[i]; i++)
            free(cmd->args[i]);
        free(cmd->args);
    }
    if (cmd->cmd_path)
        free(cmd->cmd_path);
    t_redir *redir = cmd->redirections;
    while (redir)
    {
        t_redir *tmp = redir->next;
        if (redir->filename)
            free(redir->filename);
        free(redir);
        redir = tmp;
    }
    free(cmd);
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
