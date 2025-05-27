#include "minishell.h"

static void	delete_node(t_env **node)
{
    free((*node)->key);
    free(*node);
    *node = NULL;
}

static void	remove_from_minienv(char *varname, t_env **minienv)
{
    t_env	*current;
    t_env	*prev;

    current = *minienv;
    prev = NULL;
    while (current)
    {
        if (ft_strncmp(current->key, varname, ft_strlen(varname)) == 0 &&
            current->key[ft_strlen(varname)] == '=')
        {
            if (prev)
                prev->next = current->next;
            else
                *minienv = current->next;
            delete_node(&current);
            return;
        }
        prev = current;
        current = current->next;
    }
}


int	ft_unset(char **args, t_env **minienv)
{
    int	exit_status;

    exit_status = EXIT_SUCCESS;
    while (*(++args))
    {
        if (!is_valid_varname(*args))
        {
            print_error_msg("unset", *args);
            exit_status = EXIT_FAILURE;
        }
        else
            remove_from_minienv(*args, minienv);
    }
    return (exit_status);
}