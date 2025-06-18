
#include "minishell.h"

static int	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

static void	construct_path(char *destination, const char *part1, const char *part2, const char *part3)
{
    destination[0] = '\0';
    ft_strlcat(destination, part1, PATH_MAX);
    ft_strlcat(destination, part2, PATH_MAX);
    ft_strlcat(destination, part3, PATH_MAX);
}

static int	is_absolute_or_relative_path(const char *command)
{
    return (command[0] == '/' ||
            (command[0] == '.' && command[1] == '/') ||
            (command[0] == '.' && command[1] == '.' && command[2] == '/'));
}

static char	*search_in_path_env(char *command, char *path_env, t_ctx *ctx)
{
    char	**paths;
    char	**paths_start;
    char	current_path[PATH_MAX];

    paths = ft_split(path_env, ':');
    paths_start = paths;
    while (paths && *paths)
    {
        construct_path(current_path, *paths, "/", command);
        if (access(current_path, F_OK) == 0)
        {
            if (is_directory(current_path)) {
            
            } else if (access(current_path, X_OK) == 0) {
                free_string_array(paths_start);
                return (ft_strdup(current_path));
            } else {
            
                ctx->exit_status = 126;
                free_string_array(paths_start);
                return (NULL);
            }
        }
        paths++;
    }
    if (paths_start)
        free_string_array(paths_start);
    ctx->exit_status = 127;
    errno = ENOENT;
    return (NULL);
}

char	*get_path(char *command, t_ctx *ctx)
{
    char	*path_env;


    if (is_absolute_or_relative_path(command))
	{
		if (is_directory(command))
		{
			ctx->exit_status = 126;
            errno = EISDIR;
			return (NULL);
		}
	
		if (access(command, F_OK) == 0) {
            if (access(command, X_OK) == 0)
                return (ft_strdup(command));
            else {
                ctx->exit_status = 126;
                return (NULL);
            }
        } else {
            ctx->exit_status = 127;
            errno = ENOENT;
            return (NULL);
        }
	}


    path_env = get_env_value(ctx, "PATH");
    if (!path_env || *path_env == '\0')
	{
		ctx->exit_status = 127;
        errno = ENOENT;
        return (NULL);
	}
    return (search_in_path_env(command, path_env, ctx));
}