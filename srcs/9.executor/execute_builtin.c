#include "minishell.h"

/**
 * @brief Executa um comando built-in.
 * * A assinatura e a lógica foram ajustadas para corresponderem ao minishell.h.
 * @param args O array de argumentos, onde args[0] é o comando.
 * @param ctx O contexto do minishell.
 * @return O status de saída do built-in.
 */
int	execute_builtin(char **args, t_ctx *ctx)
{
	if (!args || !args[0])
		return (0); // Ou um código de erro apropriado.
	if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(args, ctx)); // ft_echo precisa ser compatível
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args, ctx));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(args[0], "export") == 0)
		return (ft_export(args, ctx));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unset(args, ctx));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(ctx)); // ft_env precisa ser compatível
	if (ft_strcmp(args[0], "exit") == 0)
		return (ft_exit(args, ctx));
	return (127); // Retorna 'command not found' se não for um built-in.
}
