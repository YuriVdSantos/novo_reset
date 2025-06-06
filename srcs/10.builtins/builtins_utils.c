#include "minishell.h"

int	is_varname(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-');
}

void	minienv_update(char *name, char *value, t_env *minienv)
{
	t_env	*aux;
	char	*new_keypair;
	int		size;
	int		i;

	aux = minienv_node(name, minienv);
	if (!aux)
	{
		new_keypair = create_keypair(name, value);
		list_append(new_keypair, &minienv);
		free(new_keypair);
		return ;
	}
	free(aux->key);
	size = ft_strlen(name) + ft_strlen(value) + 2;
	new_keypair = malloc(size * sizeof(char));
	i = 0;
	while (*name)
		new_keypair[i++] = *name++;
	new_keypair[i++] = '=';
	while (*value)
		new_keypair[i++] = *value++;
	new_keypair[i] = '\0';
	aux->key = new_keypair;
}

void	list_append(char *key_pair, t_env **list)
{
	struct s_env	*new_node;
	struct s_env	*aux_node;


	new_node = malloc(sizeof(struct s_env));
	new_node->key = ft_strdup(key_pair);
	new_node->next = NULL;
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	aux_node = *list;
	while (aux_node->next)
		aux_node = aux_node->next;
	aux_node->next = new_node;
}

int	is_valid_varname(char *name)
{
    if (!((*name >= 'a' && *name <= 'z') || (*name >= 'A' && *name <= 'Z')))
        return (FALSE);
    while (*name)
    {
        if (!is_varname(*name))
            return (FALSE);
        name++;
    }
    return (TRUE);
}

char	*value_only(char *key_pair)
{
	int	i;

	i = 0;
	while (key_pair[i] != '=' && key_pair[i])
		i++;
	if (!key_pair[i])
		return (NULL);
	return (&key_pair[i + 1]);
}

char	*name_only(char *key_pair)
{
	int	i;

	i = 0;
	while (key_pair[i] != '=' && key_pair[i])
		i++;
	return (ft_substr(key_pair, 0, i));
}

char	*create_keypair(char *name, char *value)
{
	char	*key_pair;
	int		key_pair_size;
	int		i;

	key_pair_size = ft_strlen(name) + ft_strlen(value) + 2;
	key_pair = malloc(key_pair_size * sizeof(char));
	i = 0;
	while (*name)
		key_pair[i++] = *name++;
	key_pair[i++] = '=';
	while (*value)
		key_pair[i++] = *value++;
	key_pair[i] = '\0';
	return (key_pair);
}
