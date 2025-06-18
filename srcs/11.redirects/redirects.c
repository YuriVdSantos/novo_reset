#include "minishell.h"
#include <fcntl.h> // Incluído para fcntl (para depuração de FDs)

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
    // int check_fd; // Variável para depuração (comentada)

	if (redir->type == REDIR_OUTPUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else // REDIR_APPEND
		flags = O_WRONLY | O_CREAT | O_APPEND;

	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		print_error(NULL, redir->filename, errno, 1);
		return (FAILED); // Falha ao abrir o arquivo
	}

	// Linhas de depuração (descomente e recompile para usá-las)
	// check_fd = fcntl(STDOUT_FILENO, F_GETFD);
	// ft_printf("DEBUG: Antes do dup2, STDOUT_FILENO é %d\n", check_fd);
	// ft_printf("DEBUG: Redirecionando %s (fd=%d) para STDOUT_FILENO\n", redir->filename, fd);

	if (redirect_fd(fd, STDOUT_FILENO) == FAILED)
	{
        // Este caso implica que dup2 falhou após a abertura bem-sucedida do arquivo
        // A mensagem de erro para falha de dup2 é impressa por redirect_fd
		return (FAILED);
	}

    // Linhas de depuração (descomente e recompile para usá-las)
	// check_fd = fcntl(STDOUT_FILENO, F_GETFD);
	// ft_printf("DEBUG: Após o dup2, STDOUT_FILENO é %d\n", check_fd);
    // ft_printf("DEBUG: O novo STDOUT_FILENO é para %s\n", redir->filename); // Isso pode precisar de fcntl(new_fd, F_GETPATH) que é específico do Linux

	return (SUCCESS); // Tanto a abertura quanto o dup2 foram bem-sucedidos
}

int	apply_redirections(t_cmd *cmd, int original_fds[2])
{
	t_redir	*redir;

	redir = cmd->redirections;
	original_fds[IN] = NO_REDIRECT; //
	original_fds[OUT] = NO_REDIRECT; //
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