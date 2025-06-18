
#include "minishell.h"

void	handle_assignment_var(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	char	*value;
	int		i;

	i = 0;
	while ((*tmp)->value && (*tmp)->value[i] && (*tmp)->value[i] != '=')
		i++;
	value = ft_strdup((*tmp)->value + i + 1);
	add_arg(current, value);
	set_env_var(ctx, (*tmp)->value);
	*tmp = (*tmp)->next;
}

void	handle_env_var(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	char	*env_value;

	env_value = get_env_value(ctx, (*tmp)->value);
    if (env_value == NULL || ft_strlen(env_value) == 0)
    {
        *tmp = (*tmp)->next; 
        return; 
    }
	add_arg(current, safe_strdup(ctx, env_value));
	current->type = CMD_BUILTIN; 
	*tmp = (*tmp)->next;
}

void	handle_parse_error(t_ctx *ctx, t_token **tmp)
{
	print_error(ctx, "syntax error", -1, 2);
	*tmp = (*tmp)->next;
}