/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 21:59:16 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/26 21:59:49 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

static int	handle_input_redir(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		print_error(NULL, redir->filename, errno, 1);
		return (FAILED);
	}
	if (redirect_fd(fd, STDIN_FILENO) == FAILED)
		return (FAILED);
	return (SUCCESS);
}

static int	handle_output_redir(t_redir *redir)
{
	int	fd;
	int	flags;

	if (redir->type == REDIR_OUTPUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		print_error(NULL, redir->filename, errno, 1);
		return (FAILED);
	}
	if (redirect_fd(fd, STDOUT_FILENO) == FAILED)
		return (FAILED);
	return (SUCCESS);
}

int	apply_redirections(t_cmd *cmd, int original_fds[2])
{
	t_redir	*redir;

	redir = cmd->redirections;
	original_fds[IN] = NO_REDIRECT;
	original_fds[OUT] = NO_REDIRECT;
	while (redir)
	{
		if (redir->type == REDIR_INPUT || redir->type == REDIR_HEREDOC)
		{
			save_original_fd_in(original_fds);
			if (handle_input_redir(redir) == FAILED)
				return (FAILED);
		}
		else if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
		{
			save_original_fd_out(original_fds);
			if (handle_output_redir(redir) == FAILED)
				return (FAILED);
		}
		redir = redir->next;
	}
	return (SUCCESS);
}
