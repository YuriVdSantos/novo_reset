/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:31:43 by jhualves          #+#    #+#             */
/*   Updated: 2025/06/27 15:31:44 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	path_env = get_env_value(ctx, "PATH");
	if (!path_env)
	{
		return (NULL);
	}
	paths = ft_split(path_env, ':');
	full_path = find_in_paths(paths, command);
	free_array(paths);
	return (full_path);
}
