#include "../../minishell.h"

static int	is_folder(const char *command)
{
	struct stat	statbuf;

	if (stat(command, &statbuf) == 0)
	{
		return (S_ISDIR(statbuf.st_mode));
	}
	return (FALSE);
}

static void	handle_execve_errors(char **args, const char *path)
{
	int	error_code;

	(void)path;
	print_error_msg(args[0], strerror(errno));
	if (errno == ENOENT)
		error_code = CMD_NOT_FOUND;
	else if (errno == EACCES)
		error_code = NOT_EXECUTABLE;
	else
		error_code = EXIT_FAILURE;
	close_all_fds();
	exit(error_code);
}

void	external_exit(char **args, int exit_status)
{
	if (exit_status == NOT_EXECUTABLE)
		print_error_msg(args[0], NOT_EXECUTABLE_MSG);
	else if (exit_status == CMD_NOT_FOUND)
		print_error_msg(args[0], CMD_NOT_FOUND_MSG);
	close_all_fds();
	exit(exit_status);
}

int	execute_external(char **args, t_env *minienv, t_ctx *ctx)
{
	const char	*path;
	char		*command;
	char		**envp;

	(void)minienv;
	command = args[0];
	if (is_empty(command))
		external_exit(args, EXIT_SUCCESS);
	if (is_folder(command))
		external_exit(args, NOT_EXECUTABLE);
	path = get_path(command, ctx);
	if (path == NULL)
	{
		external_exit(args, CMD_NOT_FOUND);
	}
	rl_clear_history();
	close_extra_fds();
	envp = ctx->env_list_str;
	if (execve(path, args, envp) == -1)
		handle_execve_errors(args, path);
	exit(EXIT_FAILURE);
}