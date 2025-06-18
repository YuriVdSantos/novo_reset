#include "minishell.h"

#ifndef GNL_BUFFER_SIZE
# define GNL_BUFFER_SIZE 1024
#endif

static int	find_newline(const char *s)
{
	int	i;

	if (!s)
		return (-1);
	i = 0;
	while (s[i])
	{
		if (s[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

static int	read_to_buffer(int fd, char **stash)
{
	char	*buffer;
	int		bytes_read;
	char	*temp;

	buffer = (char *)malloc(sizeof(char) * (GNL_BUFFER_SIZE + 1));
	if (!buffer)
		return (-1);
	bytes_read = 1;


	while (find_newline(*stash) == -1 && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, GNL_BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			return (-1);
		}
		buffer[bytes_read] = '\0';
		if (*stash == NULL)
			*stash = ft_strdup(buffer);
		else
		{
			temp = ft_strjoin(*stash, buffer);
			free(*stash);
			*stash = temp;
		}
		if (*stash == NULL)
		{
			free(buffer);
			return (-1);
		}
	}
	free(buffer);
	return (bytes_read);
}

char	*get_next_line_simplified(int fd)
{
	static char	*stash[1024];
	char		*line;
	int			newline_pos;
	char		*temp;
	int			read_result;

	if (fd < 0 || GNL_BUFFER_SIZE <= 0)
		return (NULL);
	read_result = read_to_buffer(fd, &stash[fd]);
	if (read_result == -1)
	{
		if (stash[fd])
		{
			free(stash[fd]);
			stash[fd] = NULL;
		}
		return (NULL);
	}
	if (stash[fd] == NULL || stash[fd][0] == '\0')
	{
		if (stash[fd])
		{
			free(stash[fd]);
			stash[fd] = NULL;
		}
		return (NULL);
	}
	newline_pos = find_newline(stash[fd]);
	if (newline_pos != -1)
	{
		line = ft_strndup(stash[fd], newline_pos + 1);
		temp = ft_strdup(stash[fd] + newline_pos + 1);
		free(stash[fd]);
		stash[fd] = temp;
		if (stash[fd][0] == '\0')
		{
			free(stash[fd]);
			stash[fd] = NULL;
		}
	}
	else
	{
		line = ft_strdup(stash[fd]);
		free(stash[fd]);
		stash[fd] = NULL;
	}
	return (line);
}
