#include "../../minishell.h"

static void	replace_spaces_with_placeholder(char *str, char delimiter);
static void	restore_placeholder_to_spaces(char **exec_args);
static int	has_quote_mark(char *str);
static void	remove_all_quotes(char *str);
// ft_strchr já existe na libft para verificar espaços

// Adicionando t_ctx *ctx como parâmetro
char	**split_args(t_ctx *ctx, char *command)
{
    char	**exec_args;

    // Se a string 'command' não contiver aspas (pois já foram removidas)
    // E contiver espaços, isso indica que era originalmente uma única string
    // entre aspas que não deve ser dividida em múltiplos argumentos.
    // Neste caso, retorne-a como um único elemento no array.
    if (!has_quote_mark(command) && ft_strchr(command, ' '))
    {
        // Aloca espaço para um array de um único ponteiro de string + NULL
        // Corrigido: Adicionado o terceiro argumento 'TEMP' para safe_malloc
        exec_args = (char **)safe_malloc(ctx, sizeof(char *) * 2, 8);
        if (!exec_args) return (NULL); // Tratamento de erro para alocação

        // Duplica a string original para ser o único argumento
        exec_args[0] = safe_strdup(ctx, command);
        if (!exec_args[0])
        {
            free(exec_args); // Libera o array se a duplicação falhar
            return (NULL);
        }
        exec_args[1] = NULL; // Termina o array com NULL
        return (exec_args);
    }
    
    // Se a string não contiver aspas (e não contiver espaços, ou seja, é uma palavra simples)
    // ou se ela ainda contiver aspas (indicando que é a linha de comando original com aspas),
    // siga a lógica existente.
    if (!has_quote_mark(command))
        return (ft_split(command, ' '));
    
    // Esta parte do código deve ser atingida apenas se 'command' ainda contiver aspas,
    // e for a linha de comando original bruta.
    replace_spaces_with_placeholder(command, '"');
    replace_spaces_with_placeholder(command, '\'');
    remove_all_quotes(command);
    exec_args = ft_split(command, ' ');
    restore_placeholder_to_spaces(exec_args);
    return (exec_args);
}

static void	restore_placeholder_to_spaces(char **exec_args)
{
    char	*current_str;

    while (*exec_args)
    {
        current_str = *exec_args;
        while (*current_str)
        {
            if (*current_str == -1) // -1 é o placeholder para espaço
                *current_str = ' ';
            current_str++;
        }
        exec_args++;
    }
}

static void	remove_all_quotes(char *str)
{
    char	open_quote;

    open_quote = 0;
    while (*str)
    {
        if (!open_quote && (*str == '\'' || *str == '"'))
        {
            open_quote = *str;
            ft_memmove(str, str + 1, ft_strlen(str + 1) + 1);
        }
        else if (open_quote && *str == open_quote)
        {
            open_quote = 0;
            ft_memmove(str, str + 1, ft_strlen(str + 1) + 1);
        }
        else
            str++;
    }
}

static void	replace_spaces_with_placeholder(char *str, char delimiter)
{
    while (*str)
    {
        if (*str == delimiter)
        {
            str++;
            while (*str && *str != delimiter)
            {
                if (*str == ' ')
                    *str = -1; // -1 como placeholder
                str++;
            }
        }
        if (*str)
            str++;
    }
}

static int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static int	has_quote_mark(char *str)
{
    while (str && *str)
    {
        if (is_quote(*str))
            return (1);
        str++;
    }
    return (0);
}
