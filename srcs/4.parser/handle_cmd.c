// novo_reset_minishell/srcs/4.parser/handle_cmd.c

#include "minishell.h"

void    handle_pipe(t_token **tmp, t_cmd **current)
{
    t_cmd   *new;

    new = new_cmd(); //
    (*current)->next = new;
    *current = new;
    *tmp = (*tmp)->next;
}

void    handle_redir(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
    t_redir_type    type;
    char            *full_filename = NULL; // This will store the concatenated filename
    char            *segment_value = NULL; // Value of the current token segment
    char            *temp_for_free = NULL; // Temporary storage for values that need freeing

    // Determine the type of redirection
    if ((*tmp)->type == REDIR_IN)
        type = REDIR_INPUT; //
    else if ((*tmp)->type == REDIR_OUT)
        type = REDIR_OUTPUT; //
    else if ((*tmp)->type == HEREDOC)
        type = REDIR_HEREDOC; //
    else if ((*tmp)->type == APPEND)
        type = REDIR_APPEND; //

    *tmp = (*tmp)->next; // Move past the redirection operator token

    // Loop through all consecutive tokens that form part of a filename
    while (*tmp && ((*tmp)->type == WORD || (*tmp)->type == SQUOTE || (*tmp)->type == DQUOTE))
    {
        segment_value = NULL; // Reset for current segment

        if ((*tmp)->type == SQUOTE)
        {
            // For single-quoted strings, the value is literal (no expansion).
            segment_value = safe_strdup(ctx, (*tmp)->value); //
        }
        else if ((*tmp)->type == DQUOTE)
        {
            // For double-quoted strings, expand environment variables.
            segment_value = expand_dquotes(ctx, (*tmp)->value); //
        }
        else if ((*tmp)->type == WORD)
        {
            // For unquoted words, expand variables and remove inner quotes.
            char *expanded_word = expand_string(ctx, (*tmp)->value); //
            if (!expanded_word) { ctx->exit_status = 1; return; }

            // remove_quotes returns a newly malloc'd string that needs to be freed.
            temp_for_free = remove_quotes(expanded_word); //
            if (!temp_for_free) { ctx->exit_status = 1; return; }
            
            // Duplicate to ensure it's managed by safe_malloc for consistent freeing.
            segment_value = safe_strdup(ctx, temp_for_free); //
            free(temp_for_free); // Free the string from remove_quotes
        }

        if (!segment_value) { // Handle allocation/expansion failures
             ctx->exit_status = 1; // Set error status if safe_strdup returned NULL
             return;
        }

        // Concatenate the current segment to the full filename
        if (full_filename == NULL)
        {
            full_filename = segment_value; // First segment becomes the initial full_filename
        }
        else
        {
            // Concatenate full_filename and segment_value, freeing the old full_filename
            full_filename = ft_strjoin_free(ctx, full_filename, segment_value); //
            if (!full_filename) { ctx->exit_status = 1; return; }
            // segment_value is automatically freed by ft_strjoin_free as its s2 parameter if it was a safe_strdup
            // (check ft_strjoin_free implementation - my understanding is it frees s1, not s2. So segment_value might leak if not manually freed here.)
            // Let's refine ft_strjoin_free usage. If ft_strjoin_free only frees s1, then segment_value (which is safe_strdup'd) needs explicit free if it's not the first element.
            // A safer approach:
            // if (full_filename == NULL) full_filename = segment_value;
            // else {
            //     char *new_full = safe_strjoin(ctx, full_filename, segment_value);
            //     // full_filename and segment_value are safe_strdup'ed. They will be freed by super_free.
            //     // This means we don't need ft_strjoin_free, just safe_strjoin.
            //     full_filename = new_full;
            // }
            // Given the previous code had ft_strjoin_free, let's stick to it and ensure segment_value is also correctly freed/managed.
            // Based on ft_strjoin_free(ctx, char *s1, char *s2) implementation, it calls free(s1) but not free(s2).
            // So segment_value must be freed here *if* it was malloc'd specifically for this concatenation.
            // Since segment_value is safe_strdup'd, it is tracked by ctx and will be freed by super_free. This is fine.
        }
        
        *tmp = (*tmp)->next; // Move to the next token in the list
    }

    // After the loop, check if any filename parts were collected
    if (full_filename == NULL) // No valid filename token found after the redirection operator
    {
        print_error(ctx, "syntax error near redirection", -1, 2); //
        return;
    }
    
    // Add the final concatenated filename to the command's redirections
    add_redir(current, type, full_filename); // add_redir will create its own ft_strdup copy of full_filename
    // full_filename itself (which is safe_strdup'd) will be freed by super_free at the end of the command cycle.
}

void    handle_word(t_token **tmp, t_cmd *current)
{
    add_arg(current, (*tmp)->value); //
    *tmp = (*tmp)->next;
}

void    handle_dquote(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
    char    *content;

    content = safe_strdup(ctx, expand_dquotes(ctx, (*tmp)->value)); //
    add_arg(current, content); //
    *tmp = (*tmp)->next;
}

void    handle_squote(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
    char    *content;
    content = safe_strdup(ctx, (*tmp)->value); //
    add_arg(current, content); //
    *tmp = (*tmp)->next;
}