

#include "minishell.h"

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	quote_char;
	char	*new_str;

	if (!str)
		return (NULL);
	new_str = malloc(ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	quote_char = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && quote_char == 0)
		{
			quote_char = str[i];
			i++;
		}
		else if (quote_char != 0 && str[i] == quote_char)
		{
			quote_char = 0;
			i++;
		}
		else
		{
			new_str[j++] = str[i++];
		}
	}
	new_str[j] = '\0';
	return (new_str);
}


static void	process_word_token(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	char	*expanded_str;
	bool	is_unquoted_var;

	is_unquoted_var = ((*tmp)->value[0] == '$' && \
		ft_strchr((*tmp)->value, '\'') == NULL && \
		ft_strchr((*tmp)->value, '\"') == NULL);
	
	expanded_str = expand_string(ctx, (*tmp)->value);
	if (expanded_str == NULL)
	{
		ctx->exit_status = 1;
		*tmp = (*tmp)->next;
		return;
	}
	if (expanded_str[0] == '\0' && is_unquoted_var)
	{
		*tmp = (*tmp)->next;
		return;
	}
	add_arg(current, expanded_str);
	*tmp = (*tmp)->next;
}

t_cmd	*parse_tokens(t_ctx *ctx, t_token **tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	t_token	*tmp;

	head = new_cmd();
	current = head;
	tmp = *tokens;
	while (tmp && tmp->type != END)
	{
		if (tmp->type == PIPE)
			handle_pipe(&tmp, &current);
		else if (tmp->type >= REDIR_IN && tmp->type <= APPEND)
			handle_redir(ctx, &tmp, current);
		else if (tmp->type == WORD)
			process_word_token(ctx, &tmp, current);
		else if (tmp->type == DQUOTE)
             handle_dquote(ctx, &tmp, current);
        else if (tmp->type == SQUOTE)
             handle_squote(ctx, &tmp, current);
        else if (tmp->type == ENV_VAR)
             handle_env_var(ctx, &tmp, current);
		else
		{
			handle_error(ctx, "Parsing error: unexpected token", -1, 2);
			tmp = tmp->next;
		}
	}
	return (head);
}

int	only_var_assignments(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->type != END && current->type != PIPE)
	{
		if (current->type != WORD || !ft_strchr(current->value, '='))
			return (0);
		current = current->next;
	}
	return (1);
}

void	set_type_word(t_token *tokens)
{
	(void)tokens;
}