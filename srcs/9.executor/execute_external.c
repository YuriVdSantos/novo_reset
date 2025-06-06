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
static char	**convert_env_list_to_array(t_env *env_list)
{
	char	**envp;
	t_env	*current;
	int		count;
	int		i;
	char	*temp;

	count = 0;
	current = env_list;
	while (current && ++count)
		current = current->next;
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		temp = ft_strjoin(current->key, "=");
		envp[i] = ft_strjoin(temp, current->value);
		free(temp);
		if (!envp[i])
			return (free_string_array(envp), NULL);
		i++;
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
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
	path = getenv("PATH");
	// path = get_path(command, ctx);
	// printf("Executing external command: %s\n", path);
	if (path == NULL && minienv_has_path(ctx))
		external_exit(args, minienv, CMD_NOT_FOUND);
	else if (path == NULL)
		path = ft_strdup(command);
	rl_clear_history();
	close_extra_fds();
	envp = convert_env_list_to_array(minienv);
	printf("Path: '%s'\n", path);
	printf("Args: %s\n", args[0]);
	for (int i = 0; envp[i] != NULL; i++)
	{
		printf("envp[%d]: %s\n", i, envp[i]);
	}

	if (execve(path, args, envp) == -1)
		handle_execve_errors(args, path, envp);
	exit(EXIT_SUCCESS);
}