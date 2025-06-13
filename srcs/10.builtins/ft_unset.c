// #include "minishell.h"

// static void	delete_node(t_env **node)
// {
//     free((*node)->key);
//     free(*node);
//     *node = NULL;
// }

// static void	remove_from_minienv(char *varname, t_env **minienv)
// {
//     t_env	*current;
//     t_env	*prev;

//     current = *minienv;
//     prev = NULL;
//     while (current)
//     {
//         if (ft_strncmp(current->key, varname, ft_strlen(varname)) == 0 &&
//             current->key[ft_strlen(varname)] == '=')
//         {
//             if (prev)
//                 prev->next = current->next;
//             else
//                 *minienv = current->next;
//             delete_node(&current);
//             return;
//         }
//         prev = current;
//         current = current->next;
//     }
// }


// int	ft_unset(char **args, t_env **minienv)
// {
//     int	exit_status;

//     exit_status = EXIT_SUCCESS;
//     while (*(++args))
//     {
//         if (!is_valid_varname(*args))
//         {
//             print_error_msg("unset", *args);
//             exit_status = EXIT_FAILURE;
//         }
//         else
//             remove_from_minienv(*args, minienv);
//     }
//     return (exit_status);
// }

#include "minishell.h"

/*
** Verifica se uma string é um identificador válido para variáveis de shell.
*/
static int	is_valid_identifier(const char *name)
{
	if (!name || (!ft_isalpha(*name) && *name != '_'))
		return (0);
	name++;
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

/*
** Encontra e remove uma variável de ambiente da lista.
** Libera corretamente a chave, o valor e o próprio nó.
*/
static void	remove_env_var(t_env **env_list, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

/*
** Remove variáveis de ambiente.
** Itera sobre os argumentos, valida-os e os remove da lista de ambiente.
*/
int	ft_unset(char **args, t_ctx *ctx)
{
	int	exit_status;
	int i;

	exit_status = EXIT_SUCCESS;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = EXIT_FAILURE;
		}
		else
		{
			remove_env_var(&ctx->env_list, args[i]);
		}
		i++;
	}
	return (exit_status);
}
