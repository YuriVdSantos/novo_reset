#include "../../minishell.h"

static int is_folder(const char *command)
{
    struct stat statbuf;

    if (stat(command, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
    {
        while (*command == '.' || *command == '/')
            command++;
        return (*command == '\0');
    }
    return FALSE;
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
	(void)envp;
	(void)args;


	// free_string_array(envp);
	// free_string_array(args);

    exit(error_code);
}

void	external_exit(char **args, t_env *minienv, int exit_status)
{
	if (exit_status == NOT_EXECUTABLE)
		print_error_msg(args[0], NOT_EXECUTABLE_MSG);
	if (exit_status == CMD_NOT_FOUND)
		print_error_msg(args[0], CMD_NOT_FOUND_MSG);
	(void)minienv;
	// free_env_list(minienv);
	close_all_fds();
	exit(exit_status);
}

static int	minienv_has_path(t_ctx *minienv)
{
	return (get_env_value(minienv, "PATH") != NULL);
}
// static char	**convert_env_list_to_array(t_env *env_list)
// {
// 	char	**envp;
// 	t_env	*current;
// 	int		count;
// 	int		i;
// 	char	*temp;

// 	while (env_list && env_list->next)
// 	{
// 		// printf("Key: %s, Value: %s\n", env_list->key, env_list->value);
// 		env_list = env_list->next;
// 	}
// 	// printf("Eu sou o convert_env_list_to_array\n");
// 	count = 0;
// 	// printf("ola\n");
// 	current = env_list;
// 	// printf("current: %s\n", current->key);
// 	while (current && ++count)
// 		current = current->next;
// 	// printf("Count: %d\n", count);
// 	envp = malloc(sizeof(char *) * (count + 1));
// 	if (!envp)
// 		return (NULL);
// 	i = 0;
// 	current = env_list;
// 	while (current)
// 	{
// 		temp = ft_strjoin(current->key, "=");
// 		envp[i] = ft_strjoin(temp, current->value);
// 		free(temp);
// 		if (!envp[i])
// 		{
// 			free_string_array(envp);
// 			return (NULL);
// 		}
// 		i++;
// 		current = current->next;
// 	}
// 	envp[i] = NULL;
// 	// for (i = 0; i < count; i++)
// 	// 	{
// 	// 		printf("%s\n", envp[i]);
// 	// 	}
// 	return (envp);
// }


int	execute_external(char **args, t_env *minienv, t_ctx *ctx)
{
	char	const	*path;
	char			*command;
	char	**envp;
	t_env	*tmp;

	// printf("passei por aqui\n");
	command = args[0];
	if (is_empty(command))
	{
		external_exit(args, minienv, EXIT_SUCCESS);
		// printf("Command is empty\n");
	}
	if (is_folder(command))
		external_exit(args, minienv, NOT_EXECUTABLE);
	// path = getenv("PATH");
	path = get_path(command, ctx);
	// printf("Path: %s\n", path);
	// printf("Executing external command: %s\n", path);
	if (path == NULL && minienv_has_path(ctx))
		external_exit(args, minienv, CMD_NOT_FOUND);
	else if (path == NULL)
		path = ft_strdup(command);
	rl_clear_history();
	close_extra_fds();
	// printf("socorro\n");
	tmp = minienv;
	while (tmp && tmp->next)
	{
		// printf("Key: %s, Value: %s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	// envp = convert_env_list_to_array(minienv);
	envp = ctx->env_list_str;
	// printf("Path: '%s'\n", path);
	// printf("Args: %s\n", args[0]);
	if (execve(path, args, envp) == -1)
		handle_execve_errors(args, path, envp);
	free_string_array(envp);
	exit(EXIT_SUCCESS);
}
