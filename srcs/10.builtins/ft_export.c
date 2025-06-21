#include "minishell.h"

// Função para imprimir as variáveis de ambiente no formato 'declare -x KEY="VALUE"'
// Esta é a sua função original.
static int	print_export_env(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(current->key, STDOUT_FILENO);
		if (current->value != NULL)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Valida se um argumento para 'export' é um identificador válido.
 *
 * Um identificador válido para export deve começar com uma letra ou underscore,
 * seguido por letras, números ou underscores. Esta função verifica apenas a
 * parte da CHAVE (antes do '=') para validação.
 *
 * @param arg A string do argumento (ex: "VAR=valor" ou "VAR").
 * @return 1 se for válido, 0 caso contrário.
 */
static int	is_valid_export_argument(const char *arg)
{
	int	i;

	i = 0;
	// O primeiro caractere deve ser uma letra ou um underscore.
	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (0);
	i++;
	// Itera até o final da chave (encontra '=' ou o fim da string).
	while (arg[i] && arg[i] != '=')
	{
		// Os caracteres restantes da chave devem ser alfanuméricos ou underscore.
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	// Se chegamos até aqui, o nome da variável é válido.
	return (1);
}

/**
 * @brief Adiciona ou atualiza variáveis de ambiente.
 *
 * Se chamado sem argumentos, imprime as variáveis de ambiente.
 * Para cada argumento, valida se é um identificador válido e, em caso
 * afirmativo, o adiciona/atualiza no ambiente.
 *
 * @param args Argumentos do comando (ex: {"export", "VAR=valor", "VAR2", NULL}).
 * @param ctx Ponteiro para o contexto do minishell.
 * @return EXIT_SUCCESS ou EXIT_FAILURE.
 */
int	ft_export(char **args, t_ctx *ctx)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = EXIT_SUCCESS;
	// Se não houver argumentos, imprime o ambiente.
	if (!args[i])
	{
		print_export_env(ctx->env_list);
		return (exit_status);
	}
	// Itera sobre todos os argumentos.
	while (args[i])
	{
		// Usa a nova função de validação que entende o formato "CHAVE=VALOR".
		if (!is_valid_export_argument(args[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = EXIT_FAILURE;
		}
		else
		{
			// set_env_var já lida com os casos de ter ou não um '='.
			// Ex: "VAR=valor" e "VAR".
			set_env_var(ctx, args[i]);
		}
		i++;
	}
	return (exit_status);
}
