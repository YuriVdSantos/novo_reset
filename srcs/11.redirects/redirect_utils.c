#include "minishell.h"

// Modificado para retornar um int indicando sucesso (SUCCESS) ou falha (FAILED)
int	redirect_fd(int fd_to_redirect, int fd_location)
{
	if (dup2(fd_to_redirect, fd_location) == -1) // This check is key!
	{
		print_error(NULL, "dup2 failed", errno, 1);
		close(fd_to_redirect);
		return (FAILED); // Correctly returns FAILED if dup2 fails
	}
	close(fd_to_redirect);
	return (SUCCESS); // Correctly returns SUCCESS if dup2 succeeds
}

void	restore_original_fds(int original_fds[2])
{
	if (original_fds[IN] != NO_REDIRECT) //
	{
		redirect_fd(original_fds[IN], STDIN_FILENO); //
		// Removido close(original_fds[IN]); pois redirect_fd já o faz
	}
	if (original_fds[OUT] != NO_REDIRECT) //
	{
		redirect_fd(original_fds[OUT], STDOUT_FILENO); //
		// Removido close(original_fds[OUT]); pois redirect_fd já o faz
	}
}

void	save_original_fd_in(int original_fds[2])
{
	if (original_fds[IN] == NO_REDIRECT) //
		original_fds[IN] = dup(STDIN_FILENO);
}

void	save_original_fd_out(int original_fds[2])
{
	if (original_fds[OUT] == NO_REDIRECT) //
		original_fds[OUT] = dup(STDOUT_FILENO);
}
