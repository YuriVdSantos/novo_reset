// novo_reset_minishell/srcs/9.executor/execute_external.c

#include "../../minishell.h"

// Removida a função is_folder, pois sua lógica é tratada em get_path.

static void	handle_execve_errors(char **args, const char *path, t_ctx *ctx) // Adicionado t_ctx *ctx
{
	(void)path; // path pode ser NULL
	// Usa print_error, que é o manipulador de erros central.
	// errno é definido pela falha de execve ou por get_path.
	print_error(ctx, args[0], errno, ctx->exit_status);
	close_all_fds();
	exit(ctx->exit_status); // Sai com o status já definido em ctx
}

// Removida a função external_exit. Sua lógica agora está integrada em handle_execve_errors e execute_external.

int	execute_external(char **args, t_env *minienv, t_ctx *ctx)
{
	const char	*path;
	char		*command;
	char		**envp;

	(void)minienv;
	command = args[0];

	if (is_empty(command))
	{
		close_all_fds();
		exit(EXIT_SUCCESS);
	}

	path = get_path(command, ctx); // get_path define ctx->exit_status e errno
	if (path == NULL) // get_path retornou NULL, indicando um erro
	{
		// A mensagem de erro e o status de saída já estão definidos em ctx por get_path.
		// Chama handle_execve_errors para imprimir e sair.
		handle_execve_errors(args, path, ctx);
	}

	rl_clear_history(); // Limpa o histórico no processo filho
	close_extra_fds();  // Fecha quaisquer descritores de arquivo extras herdados

	envp = ctx->env_list_str; // Obtém variáveis de ambiente como char**

	if (execve(path, args, envp) == -1)
	{
		// Se execve falhar, errno já está definido pela chamada do sistema.
		// Determina o status de saída correto se não foi definido por get_path.
		if (errno == ENOENT)
			ctx->exit_status = 127;
		else if (errno == EACCES)
			ctx->exit_status = 126;
		else // Outros erros de execve
			ctx->exit_status = EXIT_FAILURE;

		handle_execve_errors(args, path, ctx); // Imprime erro e sai
	}

	// Esta linha idealmente não deve ser alcançada se execve for bem-sucedido ou handle_execve_errors sair.
	exit(EXIT_FAILURE);
}