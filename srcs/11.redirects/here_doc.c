/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurivieiradossantos <yurivieiradossanto    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:59:18 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/13 12:46:59 by yurivieirad      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_temp_filename(void)
{
	char		*filename;
	static int	heredoc_count;
	char		*count_str;

	count_str = ft_itoa(heredoc_count++);
	filename = ft_strjoin("/tmp/minishell_heredoc_", count_str);
	free(count_str);
	return (filename);
}

// Em srcs/11.redirects/here_doc.c

static int handle_single_heredoc(t_redir *redir, t_ctx *ctx)
{
	int		temp_fd;
	char	*line;
	char	*delimiter;
	char	*temp_filename;
	bool	expand;

	delimiter = redir->filename;
	expand = !ft_strchr(delimiter, '\'') && !ft_strchr(delimiter, '\"');

	temp_filename = create_temp_filename();
	if (!temp_filename) // Adicionar verificação de falha de alocação
		return (FAILED);
		
	temp_fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (temp_fd == -1)
		return (free(temp_filename), FAILED);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if(line)
				free(line);
			break ;
		}
		if (expand)
		{
			char *expanded_line = expand_string(ctx, line);
			ft_putendl_fd(expanded_line, temp_fd);
			free(expanded_line);
		}
		else
			ft_putendl_fd(line, temp_fd);
		free(line);
	}
	close(temp_fd);
	free(redir->filename); // Libera a memória do delimitador antigo
	redir->filename = temp_filename; // Atribui o novo nome de arquivo temporário
	redir->type = REDIR_INPUT;
	return (SUCCESS);
}

int	process_heredocs(t_cmd *cmd_list, t_ctx *ctx)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		current_redir = current_cmd->redirections;
		while (current_redir)
		{
			if (current_redir->type == REDIR_HEREDOC)
			{
				if (handle_single_heredoc(current_redir, ctx) == FAILED)
				{
					return (FAILED);
				}
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
	return (SUCCESS);
}

void	cleanup_heredocs(t_cmd *cmd_list)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		current_redir = current_cmd->redirections;
		while (current_redir)
		{
			if (current_redir->was_heredoc)
			{
				unlink(current_redir->filename);
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
}