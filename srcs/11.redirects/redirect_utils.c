
#include "minishell.h"

void	redirect_fd(int fd_to_redirect, int fd_location)
{
	dup2(fd_to_redirect, fd_location);
	close(fd_to_redirect);
}

void	restore_original_fds(int original_fds[2])
{
	if (original_fds[IN] != NO_REDIRECT)
	{
		redirect_fd(original_fds[IN], STDIN_FILENO);
		close(original_fds[IN]);
	}
	if (original_fds[OUT] != NO_REDIRECT)
	{
		redirect_fd(original_fds[OUT], STDOUT_FILENO);
		close(original_fds[OUT]);
	}
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
