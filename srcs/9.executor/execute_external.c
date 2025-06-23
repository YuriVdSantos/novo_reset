#include "minishell.h"
#include <errno.h>      // Necessário para a variável 'errno'
#include <sys/stat.h>   // Necessário para a função stat

/**
 * @brief Prints a standardized error message and exits the child process.
 * @param ctx   The shell's context for memory cleanup.
 * @param arg   The command argument that caused the error.
 * @param msg   The error message string to display.
 * @param code  The exit code to use.
 */
static void	exit_with_error(t_ctx *ctx, const char *arg, const char *msg, int code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putstr_fd((char *)msg, STDERR_FILENO);
	free_context(ctx);
	exit(code);
}

/**
 * @brief Executes an external command in a child process.
 *
 * This function resolves the command path, validates it, and then attempts
 * to execute it. It handles various error conditions like "command not found",
 * "is a directory", and "permission denied" with the correct exit codes.
 * @param args      The command and its arguments.
 * @param minienv   The environment list.
 * @param ctx       The shell's context for memory cleanup.
 * @return This function does not return on success; it exits the process.
 */
int	execute_external(char **args, t_env *minienv, t_ctx *ctx)
{
	char		*cmd_name;
	char		*path;
	char		**envp;
	struct stat	path_stat;

	if (!args || !args[0] || !args[0][0])
	{
		free_context(ctx);
		exit(0);
	}
	cmd_name = args[0];
	if (ft_strchr(cmd_name, '/'))
		path = cmd_name;
	else
		path = get_path(cmd_name, ctx);
	if (!path)
		exit_with_error(ctx, cmd_name, ": command not found\n", 127);
	if (stat(path, &path_stat) == -1)
		exit_with_error(ctx, cmd_name, ": No such file or directory\n", 127);
	if (S_ISDIR(path_stat.st_mode))
		print_error(ctx, "Is a directory", 126, 126);
	envp = minienv_to_envp(minienv);
	execve(path, args, envp);
	if (errno == EACCES)
		exit_with_error(ctx, cmd_name, ": Permission denied\n", 126);
	exit_with_error(ctx, cmd_name, ": command not found\n", 127);
	return (1);
}
