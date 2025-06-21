#include "minishell.h"

// Função auxiliar para verificar se uma string é um número válido (possivelmente com sinal)
static int	is_numeric(char *str)
{
	if (!str)
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

/**
 * @brief Implements the exit built-in command.
 * Exits the shell, optionally with a specific status code.
 * @param args The command arguments. `args[1]` can be the exit status.
 * @param ctx The shell's context, used for cleanup.
 * @return This function does not return, as it terminates the shell.
 */
int	ft_exit(char **args, t_ctx *ctx)
{
	long long	status;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args[1])
	{
		status = ctx->exit_status; // Pega o último status de saída
		free_context(ctx);
		exit(status);
	}
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		status = 2; // Código de erro para argumento não numérico
	}
	else if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1); // Retorna 1 mas não sai, como no bash
	}
	else
	{
		status = ft_atodbl(args[1]); // ft_atoll para converter long long
	}
	// A correção crucial está aqui: o status é armazenado em uma variável local
	// ANTES de liberar o contexto.
	free_context(ctx);
	exit((unsigned char)status); // Sai com o status, fazendo o cast para unsigned char
}
