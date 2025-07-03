/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:59:18 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/02 21:14:16 by yvieira-         ###   ########.fr       */
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

static void	heredoc_readline_loop(t_redir *redir, t_ctx *ctx, int temp_fd,
		bool expand)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, redir->filename) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		if (expand)
		{
			expanded_line = expand_string(ctx, line);
			ft_putendl_fd(expanded_line, temp_fd);
		}
		else
			ft_putendl_fd(line, temp_fd);
		free(line);
	}
}

static int	handle_single_heredoc(t_redir *redir, t_ctx *ctx)
{
	int		temp_fd;
	char	*temp_filename;
	int		expand;

	expand = 0;
	if ((ft_strchr(redir->filename, '\'') \
		|| ft_strchr(redir->filename, '\"')) \
			&& !ft_strchr(redir->filename, '$'))
				expand = 1;
	temp_filename = create_temp_filename();
	if (!temp_filename)
		return (FAILED);
	temp_fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (temp_fd == -1)
		return (free(temp_filename), FAILED);
	heredoc_readline_loop(redir, ctx, temp_fd, expand);
	close(temp_fd);
	free(redir->filename);
	redir->filename = temp_filename;
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
