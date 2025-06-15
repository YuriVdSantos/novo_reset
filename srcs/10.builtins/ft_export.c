/*

#include "minishell.h"

static void	print_env_variable(t_env *env_var)
{
    char *name;

    ft_putstr_fd("declare -x ", STDOUT_FILENO);
    name = name_only(env_var->key);
    ft_putstr_fd(name, STDOUT_FILENO);
    free(name);
    if (ft_strchr(env_var->key, '='))
    {
        ft_putstr_fd("=", STDOUT_FILENO);
        ft_putstr_fd("\"", STDOUT_FILENO);
        ft_putstr_fd(value_only(env_var->key), STDOUT_FILENO);
        ft_putstr_fd("\"", STDOUT_FILENO);
    }
    ft_putstr_fd("\n", STDOUT_FILENO);
}

static int	declare_env(t_env *minienv)
{
    t_env	*aux;

    aux = minienv;
    while (aux)
    {
        print_env_variable(aux);
        aux = aux->next;
    }
    return (0);
}

static int	process_argument(char *key_pair, t_ctx *ctx)
{
    char	*varname;
    int		status;

    varname = name_only(key_pair);
    status = EXIT_SUCCESS;
    if (!is_valid_varname(varname) || str_equal(key_pair, "="))
    {
        print_error_msg(varname, "not valid in this context:");
        status = EXIT_FAILURE;
    }
    else if (ctx->env_list->next)
        minienv_update(varname, value_only(key_pair), ctx->env_list);
    else
        list_append(key_pair, &ctx->env_list);
    // free(varname);
    return (status);
}

int	ft_export(char **args, t_ctx *ctx)
{
    int	exit_status;

    args++;
    exit_status = EXIT_SUCCESS;
    if (!*args)
        return (declare_env(ctx->env_list));
    while (*args)
    {
        if (process_argument(*args, ctx) == EXIT_FAILURE)
            exit_status = EXIT_FAILURE;
        args++;
    }
    return (exit_status);
}

*/


#include "minishell.h"

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

static int	print_export_env(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(current->key, STDOUT_FILENO);
		if (current->value != NULL)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

static char	*get_key_from_assignment(const char *assignment)
{
	const char *equal_pos;

	equal_pos = ft_strchr(assignment, '=');
	if (!equal_pos)
		return (ft_strdup(assignment));
	return (ft_strndup(assignment, equal_pos - assignment));
}

int	ft_export(char **args, t_ctx *ctx)
{
	int		exit_status;
	int		i;
	char	*key;

	exit_status = EXIT_SUCCESS;
	i = 1;
	if (!args[1])
		return (print_export_env(ctx->env_list));
	while (args[i])
	{
		key = get_key_from_assignment(args[i]);
		if (!is_valid_identifier(key))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = EXIT_FAILURE;
		}
		else if (ft_strchr(args[i], '='))
		{
			set_env_var(ctx, args[i]);
		}
		else if (!find_env_var(ctx->env_list, key))
		{
			add_new_env_var(ctx, ft_strdup(key), NULL, args[i]);
		}
		free(key);
		i++;
	}
	return (exit_status);
}
