#include "minishell.h"

// Declaração da variável global para que este arquivo saiba de sua existência.
extern int	g_exit_status;

/**
 * @brief Imprime uma mensagem de erro padronizada no stderr.
 * @param command O comando ou contexto que gerou o erro.
 * @param msg A mensagem de erro específica.
 */
void	print_error_msg(char *command, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (command)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(msg, STDERR_FILENO);
}

/**
 * @brief Função de erro mais completa.
 * @param ctx O contexto.
 * @param msg A mensagem de erro.
 * @param errnum O número do erro (errno).
 * @param exit_status O status de saída a ser definido.
 */
void	print_error(t_ctx *ctx, char *msg, int errnum, int exit_status)
{
	(void)errnum; // Pode ser usado com strerror no futuro.
	print_error_msg(NULL, msg);
	if (ctx)
		ctx->exit_status = exit_status;
	else
		g_exit_status = exit_status;
}

/**
 * @brief Handler de erro genérico.
 * Define o status de saída e imprime a mensagem.
 * @param ctx Contexto do minishell.
 * @param msg Mensagem de erro.
 * @param errnum Número do erro (se aplicável).
 * @param exit_status Status de saída a ser definido.
 * @return Sempre retorna `false` para sinalizar o erro.
 */
bool	handle_error(t_ctx *ctx, char *msg, int errnum, int exit_status)
{
	print_error(ctx, msg, errnum, exit_status);
	return (false);
}

/**
 * @brief Handler específico para erros de sintaxe.
 * Imprime a mensagem de erro de sintaxe e define o status de saída.
 * @param ctx Contexto do minishell.
 * @param msg Mensagem específica do erro de sintaxe (geralmente o token).
 * @return Sempre retorna `false`.
 */
bool	syntax_error(t_ctx *ctx, char *msg)
{
	char	*error_msg;

	error_msg = ft_strjoin("syntax error near unexpected token `", msg);
	error_msg = ft_strjoin_free(NULL, error_msg, "'");
	print_error(ctx, error_msg, 0, 2);
	free(error_msg);
	return (false);
}
