
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

static char	*search_in_path_env(char *command, char *path_env, t_ctx *ctx) // Adicionado t_ctx *ctx
{
    char	**paths;
    char	**paths_start;
    char	current_path[PATH_MAX];

    paths = ft_split(path_env, ':');
    paths_start = paths;
    while (paths && *paths)
    {
        construct_path(current_path, *paths, "/", command);
        if (access(current_path, F_OK) == 0) // Verifica se o arquivo existe
        {
            if (is_directory(current_path)) {
                // É um diretório, não executável via PATH.
            } else if (access(current_path, X_OK) == 0) { // Verifica permissão de execução
                free_string_array(paths_start);
                return (ft_strdup(current_path));
            } else {
                // Arquivo existe, mas sem permissão de execução. errno já está definido por access().
                ctx->exit_status = 126; // Define status de saída para Permissão Negada
                free_string_array(paths_start);
                return (NULL); // Indica erro.
            }
        }
        paths++;
    }
    if (paths_start)
        free_string_array(paths_start);
    ctx->exit_status = 127; // Comando não encontrado se não achou em PATH
    errno = ENOENT; // Define errno para "No such file or directory"
    return (NULL);
}

char	*get_path(char *command, t_ctx *ctx)
{
    char	*path_env;

    // Lida com caminhos absolutos ou relativos
    if (is_absolute_or_relative_path(command))
	{
		if (is_directory(command))
		{
			ctx->exit_status = 126; // É um diretório
            errno = EISDIR; // Define errno para "is a directory"
			return (NULL);
		}
		// Verifica se o arquivo existe e é executável
		if (access(command, F_OK) == 0) { // Verifica existência
            if (access(command, X_OK) == 0) // Verifica executabilidade
                return (ft_strdup(command));
            else { // Existe, mas não executável. errno já está definido por access().
                ctx->exit_status = 126; // Permissão negada
                return (NULL);
            }
        } else { // Não existe
            ctx->exit_status = 127; // No such file or directory
            errno = ENOENT; // Define errno para "No such file or directory"
            return (NULL);
        }
	}

    // Procura na variável de ambiente PATH
    path_env = get_env_value(ctx, "PATH");
    if (!path_env || *path_env == '\0')
	{
		ctx->exit_status = 127; // PATH não definido ou vazio, comando não encontrado
        errno = ENOENT; // Define errno
        return (NULL);
	}
    return (search_in_path_env(command, path_env, ctx)); // Passa ctx para search_in_path_env
}