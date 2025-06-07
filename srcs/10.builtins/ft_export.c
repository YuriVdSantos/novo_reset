#include "minishell.h"

static void	print_env_variable(t_env *env_var)
{
    ft_putstr_fd("declare -x ", STDOUT_FILENO);
    ft_putstr_fd(name_only(env_var->key), STDOUT_FILENO);
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
        print_error_msg("export", ERR_MISUSE_SHELL_MSG);
        status = EXIT_FAILURE;
    }
    else if (ctx->env_list->next)
        minienv_update(varname, value_only(key_pair), ctx->env_list);
    else
        list_append(key_pair, &ctx->env_list);
    free(varname);
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