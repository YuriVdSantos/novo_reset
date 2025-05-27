#include "../../minishell.h"

static int	is_folder(const char *command)
{
    struct stat	statbuf;

    if (stat(command, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
    {
        while (*command == '.' || *command == '/')
            command++;
        return (*command == '\0') ? TRUE : FALSE;
    }
    return (FALSE);
}

static void	handle_execve_errors(char **args, const char *path, char **envp)
{
    int	error_code;

    print_error_msg("execve", args[0]);
    if (access(path, F_OK) != 0)
        error_code = CMD_NOT_FOUND;
    else if (access(path, X_OK) != 0)
        error_code = NOT_EXECUTABLE;
    else
		error_code = EXIT_FAILURE;
	free_string_array(envp);
	free_string_array(args);

    exit(error_code);
}

void	external_exit(char **args, t_env *minienv, int exit_status)
{
	if (exit_status == NOT_EXECUTABLE)
		print_error_msg(args[0], NOT_EXECUTABLE_MSG);
	if (exit_status == CMD_NOT_FOUND)
		print_error_msg(args[0], CMD_NOT_FOUND_MSG);
	free_env_list(minienv);
	close_all_fds();
	exit(exit_status);
}

static int	minienv_has_path(t_ctx *minienv)
{
	return (get_env_value(minienv, "PATH") != NULL);
}

int	execute_external(char **args, t_env *minienv, t_ctx *ctx)
{
	char	const	*path;
	char			*command;
	char	**envp;

	command = args[0];
	if (is_empty(command))
		external_exit(args, minienv, EXIT_SUCCESS);
	if (is_folder(command))
		external_exit(args, minienv, NOT_EXECUTABLE);
	path = ctx->cmd_list->cmd_path;
	if (path == NULL && minienv_has_path(ctx))
		external_exit(args, minienv, CMD_NOT_FOUND);
	else if (path == NULL)
		path = ft_strdup(command);
	rl_clear_history();
	close_extra_fds();
	envp = minienv_to_envp(minienv);
	if (execve(path, args, envp) == -1)
		handle_execve_errors(args, path, envp);
	exit(EXIT_SUCCESS);
}