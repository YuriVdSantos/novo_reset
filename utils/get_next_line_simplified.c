#include "minishell.h"

// Tamanho do buffer de leitura para a função get_next_line_simplified
#ifndef GNL_BUFFER_SIZE
# define GNL_BUFFER_SIZE 1024
#endif

// Função auxiliar para encontrar o primeiro '\n' ou '\0'
static int	find_newline_or_null(char *s)
{
	int	i;

	if (!s)
		return (-1);
	i = 0;
	while (s[i] != '\n' && s[i] != '\0')
		i++;
	return (i);
}

// Função auxiliar para ler e acumular no buffer estático
static int	read_to_buffer(int fd, char **stash)
{
	char	*buffer;
	int		bytes_read;

	buffer = (char *)malloc(sizeof(char) * (GNL_BUFFER_SIZE + 1));
	if (!buffer)
		return (-1); // Erro de alocação
	bytes_read = 1;
	// Correção: Comparação de int com size_t
	while ((size_t)find_newline_or_null(*stash) == ft_strlen(*stash) && bytes_read != 0)
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
			*stash = ft_strjoin_free(NULL, *stash, buffer); // Reutilizando ft_strjoin_free, passando NULL para ctx
		if (*stash == NULL) // Erro de alocação no ft_strjoin_free
		{
			free(buffer);
			return (-1);
		}
	}
	free(buffer);
	return (bytes_read);
}

// Função simplificada para ler uma linha por vez de um fd
char	*get_next_line_simplified(int fd)
{
	static char	*stash[1024]; // Um stash por FD. Usando um array grande para evitar complexidade de hash map.
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
	if (stash[fd] == NULL || stash[fd][0] == '\0') // Nada para ler ou EOF
	{
		if (stash[fd]) // Se havia uma string vazia, liberte-a.
		{
			free(stash[fd]);
			stash[fd] = NULL;
		}
		return (NULL);
	}
	newline_pos = find_newline_or_null(stash[fd]);
	if (stash[fd][newline_pos] == '\n') // Encontrou um newline
	{
		line = ft_strndup(stash[fd], newline_pos + 1); // Incluir o '\n'
		temp = ft_strdup(stash[fd] + newline_pos + 1);
		free(stash[fd]);
		stash[fd] = temp;
		if (stash[fd][0] == '\0') // Se o restante for vazio, liberar
		{
			free(stash[fd]);
			stash[fd] = NULL;
		}
	}
	else // Não encontrou newline (última linha ou linha sem '\n')
	{
		line = ft_strdup(stash[fd]);
		free(stash[fd]);
		stash[fd] = NULL;
	}
	return (line);
}
