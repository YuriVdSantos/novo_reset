#include "minishell.h"

// Declaração da variável global para que este arquivo saiba de sua existência.
// A definição real (int g_exit_status = 0;) está em minishell.c.
// O ideal é colocar esta linha no seu arquivo minishell.h.
extern int	g_exit_status;

/**
 * @brief Espera por um processo filho e define o status de saída global.
 * @param child_pid O ID do processo a aguardar.
 * @param is_last_child Flag para pipelines (não muito usado para um comando).
 * @param ctx O contexto do minishell.
 * @return O status de saída do processo filho.
 */
int	wait_for_child(int child_pid, int is_last_child, t_ctx *ctx)
{
	int	status;

	(void)is_last_child; // Não usado em um comando único, mas mantém a assinatura.
	(void)ctx; // Não usado diretamente, mas g_exit_status é global.
	if (waitpid(child_pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFEXITED(status))
	{
		g_exit_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit: 3", STDERR_FILENO);
		else if (WTERMSIG(status) == SIGINT)
			ft_putstr_fd("\n", STDERR_FILENO);
	}
	return (g_exit_status);
}

/**
 * @brief Espera por todos os processos filhos em um pipeline.
 * @param children_pid Array com os PIDs dos filhos.
 * @param ctx O contexto do minishell.
 * @return O status de saída do último comando no pipeline.
 */
int	wait_for_children(int children_pid[1024], t_ctx *ctx)
{
	int	i;
	int	last_status;

	i = 0;
	last_status = 0;
	while (children_pid && children_pid[i] != 0)
	{
		// O último pid no array é o do último comando.
		int is_last = (children_pid[i + 1] == 0);
		last_status = wait_for_child(children_pid[i], is_last, ctx);
		i++;
	}
	return (last_status);
}
