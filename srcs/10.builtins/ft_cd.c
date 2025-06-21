#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Atualiza as variáveis de ambiente PWD e OLDPWD.
 * * Esta função é chamada após uma mudança de diretório bem-sucedida.
 * Ela primeiro busca o valor atual de PWD e o define como o novo OLDPWD.
 * Depois, obtém o diretório de trabalho atual e o define como o novo PWD.
 * * @param ctx Ponteiro para a estrutura de contexto do minishell.
 */
static void	update_pwd_vars(t_ctx *ctx)
{
	char	cwd[4096];
	t_env	*pwd_var;
	char	*assignment;

	// 1. Atualiza OLDPWD com o valor antigo de PWD
	pwd_var = find_env_var(ctx->env_list, "PWD");
	if (pwd_var && pwd_var->value)
	{
		assignment = ft_strjoin("OLDPWD=", pwd_var->value);
		if (assignment)
		{
			set_env_var(ctx, assignment);
			free(assignment);
		}
	}
	// 2. Atualiza PWD com o diretório atual
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		assignment = ft_strjoin("PWD=", cwd);
		if (assignment)
		{
			set_env_var(ctx, assignment);
			free(assignment);
		}
	}
	else
	{
		// Se getcwd falhar, é uma situação de erro.
		// O comportamento do bash aqui é complexo, mas por enquanto,
		// podemos apenas imprimir um erro.
		perror("minishell: cd: error retrieving current directory");
	}
}

/**
 * @brief Obtém o valor de uma variável de ambiente.
 * * Esta é uma função auxiliar para simplificar a obtenção do valor
 * a partir da função find_env_var.
 *
 * @param ctx O contexto do shell.
 * @param key A chave da variável de ambiente.
 * @return O valor da variável ou NULL se não for encontrada ou não tiver valor.
 */
static char	*get_env_value_from_ctx(t_ctx *ctx, const char *key)
{
	t_env	*var;

	var = find_env_var(ctx->env_list, key);
	if (var)
		return (var->value);
	return (NULL);
}

/**
 * @brief Implementação do builtin 'cd'.
 * * Muda o diretório de trabalho atual.
 * - `cd` (sem argumentos): Muda para o diretório HOME.
 * - `cd -`: Muda para o diretório anterior (OLDPWD).
 * - `cd [caminho]`: Muda para o diretório especificado.
 *
 * @param args Argumentos do comando, onde args[0] é "cd".
 * @param ctx Ponteiro para a estrutura de contexto do minishell.
 * @return EXIT_SUCCESS em caso de sucesso, EXIT_FAILURE em caso de erro.
 */
int	ft_cd(char **args, t_ctx *ctx)
{
	const char	*path;

	if (args[1] == NULL)
	{
		path = get_env_value_from_ctx(ctx, "HOME");
		if (path == NULL || *path == '\0')
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
	}
	else if (args[2] != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else if (args[1][0] == '-' && args[1][1] == '\0')
	{
		path = get_env_value_from_ctx(ctx, "OLDPWD");
		if (path == NULL || *path == '\0')
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		// O bash imprime o diretório para o qual está mudando ao usar "cd -"
		ft_putendl_fd((char *)path, STDOUT_FILENO);
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror((char *)path); // perror espera um char*, fazemos o cast
		return (EXIT_FAILURE);
	}
	update_pwd_vars(ctx);
	return (EXIT_SUCCESS);
}
