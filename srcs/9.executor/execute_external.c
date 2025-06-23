#include "minishell.h"
#include <errno.h>
#include <sys/stat.h>

static void exit_with_error(t_ctx *ctx, const char *arg, const char *msg, int code)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd((char *)arg, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putstr_fd((char *)msg, STDERR_FILENO);
    if (msg[ft_strlen(msg) - 1] != '\n')
        ft_putstr_fd("\n", STDERR_FILENO);
    free_context(ctx);
    exit(code);
}

int execute_external(char **args, t_env *minienv, t_ctx *ctx)
{
    char        *cmd_name;
    char        *path;
    char        **envp;
    struct stat path_stat;

    if (!args || !args[0] || !args[0][0])
    {
        free_context(ctx);
        exit(0);
    }
    cmd_name = args[0];
    if (ft_strchr(cmd_name, '/'))
        path = cmd_name;
    else
	{
		path = get_path(cmd_name, ctx);
	}
	if (access(cmd_name, F_OK) == 0)
	{
		if (!path)
        exit_with_error(ctx, cmd_name, "command not found", 127);
		else if (access(cmd_name, X_OK) == -1)
			exit_with_error(ctx, cmd_name, ": Permission denied\n", 126);
	}
    if (!path)
        exit_with_error(ctx, cmd_name, "command not found", 127);
    if (stat(path, &path_stat) == -1)
        exit_with_error(ctx, cmd_name, "No such file or directory", 127);
    if (S_ISDIR(path_stat.st_mode))
        exit_with_error(ctx, cmd_name, "Is a directory", 126);
    if (access(path, X_OK) != 0)
        exit_with_error(ctx, cmd_name, "Permission denied", 126);
    envp = minienv_to_envp(minienv);
    execve(path, args, envp);
    exit_with_error(ctx, cmd_name, strerror(errno), 126);
    return (1);
}