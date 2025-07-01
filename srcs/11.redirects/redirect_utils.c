/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:00:28 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/01 16:42:13 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_fd(int fd_to_redirect, int fd_location)
{
	if (dup2(fd_to_redirect, fd_location) == -1)
	{
		print_error(NULL, "dup2 failed", errno, 1);
		close(fd_to_redirect);
		return (FAILED);
	}
	close(fd_to_redirect);
	return (SUCCESS);
}

void	restore_original_fds(int original_fds[2])
{
	if (original_fds[IN] != NO_REDIRECT)
		redirect_fd(original_fds[IN], STDIN_FILENO);
	if (original_fds[OUT] != NO_REDIRECT)
		redirect_fd(original_fds[OUT], STDOUT_FILENO);
}

void	save_original_fd_in(int original_fds[2])
{
	if (original_fds[IN] == NO_REDIRECT)
		original_fds[IN] = dup(STDIN_FILENO);
}

void	save_original_fd_out(int original_fds[2])
{
	if (original_fds[OUT] == NO_REDIRECT)
		original_fds[OUT] = dup(STDOUT_FILENO);
}
