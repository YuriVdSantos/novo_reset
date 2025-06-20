#include "minishell.h"

// A variável g_exit_status é usada para armazenar o código de saída do último
// comando executado. É uma prática comum em projetos de minishell para lidar
// com o status de saída, especialmente com sinais.
int	g_exit_status = 0;

// Protótipos para funções locais neste arquivo.
void	execute(t_ctx *ctx);
void	free_ctx_between_commands(t_ctx *ctx);

int	main(int argc, char **argv, char **envp)
{
	t_ctx	*ctx;

	(void)argc;
	(void)argv;
	// Corrigido: Usa-se ft_calloc para alocar a estrutura de contexto inicial.
	// O safe_malloc não pode ser usado aqui, pois ele precisa de um contexto
	// já existente para funcionar.
	ctx = ft_calloc(1, sizeof(t_ctx));
	if (!ctx)
	{
		perror("ft_calloc failed");
		exit(EXIT_FAILURE);
	}
	init_ctx(ctx, envp);
	while (1)
	{
		// Corrigido: O nome da função para configurar os sinais é 'define_signals'.
		define_signals();
		ctx->input = readline("minishell> ");
		if (!ctx->input)
		{
			printf("exit\n");
			break ;
		}
		if (ft_strlen(ctx->input) > 0)
			add_history(ctx->input);
		// Corrigido: A função 'process_input' espera 2 argumentos e não retorna valor.
		// Ela modifica o 'ctx' internamente.
		process_input(ctx, ctx->input);
		// A execução acontece se a lista de comandos foi criada com sucesso.
		// if (ctx->cmd_list)
		// 	execute(ctx);
		// Libera a memória alocada para o ciclo do comando atual.
		free_ctx_between_commands(ctx);
	}
	// Corrigido: O nome da função para liberar o contexto é 'free_context'.
	free_context(ctx);
	// Retorna o status de saída global.
	return (g_exit_status);
}

/**
 * @brief Ponto de entrada para o módulo executor.
 * Verifica se há um ou múltiplos comandos e chama a função apropriada.
 * @param ctx A estrutura de contexto do minishell.
 */
void	execute(t_ctx *ctx)
{
	if (!ctx || !ctx->cmd_list)
		return ;
	// Se houver apenas um comando na lista.
	if (!ctx->cmd_list->next)
	{
		// Corrigido: A ordem dos argumentos para 'execute_one_command' foi ajustada.
		execute_one_command(ctx->cmd_list, ctx);
	}
	else
	{
		// Corrigido: 'execute_multiple_commands' espera a lista de comandos e o contexto.
		execute_multiple_commands(ctx->cmd_list, ctx);
	}
}

/**
 * @brief Libera a memória que é reutilizada em cada ciclo de comando.
 * Isso inclui a string de input, a lista de tokens e a lista de comandos.
 * A lista de variáveis de ambiente e o próprio contexto são preservados.
 * @param ctx O contexto do minishell.
 */
void	free_ctx_between_commands(t_ctx *ctx)
{
	if (!ctx)
		return ;
	if (ctx->input)
	{
		free(ctx->input);
		ctx->input = NULL;
	}
	if (ctx->token_list)
	{
		free_token_list(ctx->token_list);
		ctx->token_list = NULL;
	}
	if (ctx->cmd_list)
	{
		free_cmd_list(ctx->cmd_list);
		ctx->cmd_list = NULL;
	}
}
