#include "minishell.h"

// Definição da variável global de sinal.
// Esta é a única vez que ela é definida sem 'extern'.
volatile sig_atomic_t	g_signal = 0;

/**
 * @brief Handler para o sinal SIGINT (Ctrl+C).
 */
static void	handle_sigint(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	// Imprime uma nova linha para sair da linha atual.
	write(STDOUT_FILENO, "\n", 1);
	// Limpa a linha atual, move o cursor e redesenha o prompt.
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Configura os handlers de sinal para o modo interativo principal.
 * - Ctrl+C (SIGINT) exibe um novo prompt.
 * - Ctrl+\ (SIGQUIT) é ignorado.
 */
void	define_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN; // Ignora o SIGQUIT
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

// As funções abaixo são placeholders para quando você implementar a lógica
// de execução e heredoc, que precisam de um comportamento de sinal diferente.

void	define_execute_signals(int child_pid)
{
	(void)child_pid;
	// A definir: Lógica de sinais durante a execução de um comando.
}

void	define_heredoc_signals(int child_pid)
{
	(void)child_pid;
	// A definir: Lógica de sinais durante a leitura de um heredoc.
}
