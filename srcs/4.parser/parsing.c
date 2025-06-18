
// #include "minishell.h"

// char	*remove_quotes(char *str)
// {
// 	int		i;
// 	int		j;
// 	char	quote_char;
// 	char	*new_str;

// 	if (!str)
// 		return (NULL);
// 	new_str = malloc(ft_strlen(str) + 1);
// 	if (!new_str)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	quote_char = 0;
// 	while (str[i])
// 	{
// 		if ((str[i] == '\'' || str[i] == '\"') && quote_char == 0)
// 		{
// 			quote_char = str[i];
// 			i++;
// 		}
// 		else if (quote_char != 0 && str[i] == quote_char)
// 		{
// 			quote_char = 0;
// 			i++;
// 		}
// 		else
// 		{
// 			new_str[j++] = str[i++];
// 		}
// 	}
// 	new_str[j] = '\0';
// 	return (new_str);
// }


// static void	process_word_token(t_ctx *ctx, t_token **tmp, t_cmd *current)
// {
// 	char *expanded_str;
// 	char *final_str;

// 	expanded_str = expand_string(ctx, (*tmp)->value);
// 	if (!expanded_str) // Handle potential memory allocation failure from expand_string
// 	{
// 		ctx->exit_status = 1; // Set an error exit status
// 		*tmp = (*tmp)->next; // Skip to the next token
// 		return ;
// 	}

// 	final_str = remove_quotes(expanded_str); // remove_quotes creates a new malloc'd string
// 	// IMPORTANT: Do NOT free(expanded_str) here, as it's allocated via safe_strdup and managed by ctx->allocations.
// 	// It will be freed by super_free at the end of the command cycle.

// 	if (!final_str) // Handle potential memory allocation failure from remove_quotes
// 	{
// 		ctx->exit_status = 1; // Set an error exit status
// 		*tmp = (*tmp)->next; // Skip to the next token
// 		return ;
// 	}

// 	add_arg(current, final_str); // add_arg will create its own copy (ft_strdup) of final_str
// 	free(final_str); // This is correct, as final_str was allocated by a direct malloc in remove_quotes
	
// 	*tmp = (*tmp)->next;
// }

// t_cmd	*parse_tokens(t_ctx *ctx, t_token **tokens)
// {
// 	t_cmd	*head;
// 	t_cmd	*current;
// 	t_token	*tmp;

// 	head = new_cmd();
// 	current = head;
// 	tmp = *tokens;
// 	while (tmp && tmp->type != END)
// 	{
// 		if (tmp->type == PIPE)
// 			handle_pipe(&tmp, &current);
// 		else if (tmp->type >= REDIR_IN && tmp->type <= APPEND)
// 			handle_redir(ctx, &tmp, current);
// 		else if (tmp->type == WORD)
// 			process_word_token(ctx, &tmp, current);
// 		else if (tmp->type == DQUOTE)
//              handle_dquote(ctx, &tmp, current);
//         else if (tmp->type == SQUOTE)
//              handle_squote(ctx, &tmp, current);
//         else if (tmp->type == ENV_VAR)
//              handle_env_var(ctx, &tmp, current);
// 		else
// 		{
// 			handle_error(ctx, "Parsing error: unexpected token", -1, 2);
// 			tmp = tmp->next;
// 		}
// 	}
// 	return (head);
// }

// int	only_var_assignments(t_token *tokens)
// {
// 	t_token	*current;

// 	current = tokens;
// 	while (current && current->type != END && current->type != PIPE)
// 	{
// 		if (current->type != WORD || !ft_strchr(current->value, '='))
// 			return (0);
// 		current = current->next;
// 	}
// 	return (1);
// }

// void	set_type_word(t_token *tokens)
// {
// 	(void)tokens;
// }

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
	char *expanded_str; // Agora vem de safe_strdup
	char *final_str;

	expanded_str = expand_string(ctx, (*tmp)->value);
    // Se expanded_str vem de safe_strdup, não devemos liberá-lo aqui.
    // Ele será liberado por super_free.

	if (!expanded_str) return ;

	final_str = remove_quotes(expanded_str); // remove_quotes usa malloc puro
    // final_str é um malloc puro, então precisa ser liberado explicitamente.

    if (final_str && ft_strlen(final_str) == 0) // Lógica de descarte de strings vazias para WORD
    {
        free(final_str); // Libera o malloc de remove_quotes
        *tmp = (*tmp)->next;
        return;
    }

	if (!final_str) return ;

	add_arg(current, final_str); // add_arg faz ft_strdup(value)
	free(final_str); // Correto: libera o malloc de remove_quotes, pois add_arg fez uma cópia.
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