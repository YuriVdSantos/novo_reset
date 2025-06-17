#include "minishell.h"

// Tamanho do buffer de leitura para a função get_next_line_simplified
#ifndef GNL_BUFFER_SIZE
# define GNL_BUFFER_SIZE 1024
#endif

// Função auxiliar para encontrar o primeiro '\n' em uma string.
// Retorna o índice do '\n' ou -1 se não for encontrado.
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

// Função auxiliar para ler e acumular no buffer estático.
// Foi corrigida para ler corretamente mesmo quando o stash está vazio.
static int	read_to_buffer(int fd, char **stash)
{
	char	*buffer;
	int		bytes_read;
	char	*temp;

	buffer = (char *)malloc(sizeof(char) * (GNL_BUFFER_SIZE + 1));
	if (!buffer)
		return (-1); // Erro de alocação
	bytes_read = 1;
	// O laço agora continuará lendo enquanto não encontrar um '\n' no stash
	// e a leitura não tiver terminado (bytes_read != 0).
	while (find_newline(*stash) == -1 && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, GNL_BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			return (-1); // Erro de leitura
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
		if (*stash == NULL) // Erro de alocação no strdup ou strjoin
		{
			free(buffer);
			return (-1);
		}
	}
	free(buffer);
	return (bytes_read);
}

// Função simplificada para ler uma linha por vez de um fd.
// A lógica principal foi mantida, a correção está em read_to_buffer.
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
	if (read_result == -1) // Erro durante a leitura ou alocação
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
	if (newline_pos != -1) // Encontrou um newline
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
	else // Não encontrou newline (última linha)
	{
		line = ft_strdup(stash[fd]);
		free(stash[fd]);
		stash[fd] = NULL;
	}
	return (line);
}
