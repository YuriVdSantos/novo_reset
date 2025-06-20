#include "minishell.h"

// Adicionando o protótipo da função free_array, já que parece estar
// faltando no escopo deste arquivo. O ideal seria ter este protótipo
// em um arquivo de cabeçalho (como minishell.h ou um utils.h).
void	free_array(char **arr);

/**
 * @brief Executa um comando externo.
 * @param args O array de argumentos (argv), onde args[0] é o comando.
 * @param minienv A lista de variáveis de ambiente.
 * @param ctx O contexto do minishell.
 * @return Esta função não retorna em caso de sucesso, pois o processo é
 * substituído. Retorna um código de erro em caso de falha antes do execve.
 */
int	execute_external(char **args, t_env *minienv, t_ctx *ctx)
{
	char	*path;
	char	**envp;

	// 'get_path' precisa ser ajustado para receber 'ctx' também.
	path = get_path(args[0], ctx);
	if (!path)
	{
		print_error(ctx, args[0], CMD_NOT_FOUND, CMD_NOT_FOUND);
		exit(CMD_NOT_FOUND);
	}
	envp = minienv_to_envp(minienv);
	if (!envp)
	{
		free(path);
		print_error(ctx, "fatal", -1, 1);
		exit(1);
	}
	execve(path, args, envp);
	// Se execve retornar, houve um erro.
	free(path);
	free_array(envp);
	print_error(ctx, args[0], PERMISSION_DENIED, PERMISSION_DENIED);
	exit(PERMISSION_DENIED);
}
