#include "minishell.h"

static char	*find_in_paths(char **paths, const char *cmd)
{
	char	*full_path;
	char	*tmp;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

/**
 * @brief Obtém o caminho absoluto de um comando.
 * @param command O nome do comando.
 * @param ctx O contexto do minishell (para acessar o PATH).
 * @return Uma string alocada com o caminho, ou NULL se não for encontrado.
 */
char	*get_path(char *command, t_ctx *ctx)
{
	char	*path_env;
	char	**paths;
	char	*full_path;

	if (!command || !*command)
		return (NULL);
	if (ft_strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	// Corrigido: 'get_env_value' agora precisa de 'ctx'.
	path_env = get_env_value(ctx, "PATH");
	if (!path_env)
	{
		// Se o PATH não estiver definido, não há onde procurar.
		return (NULL);
	}
	// 'safe_split' seria mais seguro aqui, se o tiver.
	paths = ft_split(path_env, ':');
	full_path = find_in_paths(paths, command);
	// 'free_string_array' seria o ideal aqui.
	free_array(paths);
	return (full_path);
}
