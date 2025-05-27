#include "minishell.h"

int	ft_env(t_ctx *ctx)
{
    while (ctx->env_list && ctx->env_list->key)
    {
        if (ft_strchr(ctx->env_list->key, '='))
            ft_printf("%s\n", ctx->env_list->key);
        ctx->env_list = ctx->env_list->next;
    }
    return (EXIT_SUCCESS);
}