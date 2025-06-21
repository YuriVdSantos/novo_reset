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

void	define_interactive_signals(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Configura os handlers de sinal para o modo interativo principal.
 * - Ctrl+C (SIGINT) exibe um novo prompt.
 * - Ctrl+\ (SIGQUIT) é ignorado.
 */
void	define_execute_signals(int child_pid)
{
    if (child_pid == 0)
    {
        // Processo filho: restaurar sinais padrão
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
    }
    else
    {
        // Processo pai: ignorar sinais enquanto espera o filho
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
    }
}

void	define_heredoc_signals(int child_pid)
{
    if (child_pid == 0)
    {
        // Processo filho (heredoc): SIGINT encerra o heredoc
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_IGN);
    }
    else
    {
        // Processo pai: ignora SIGINT durante o heredoc
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
    }
}

// As funções abaixo são placeholders para quando você implementar a lógica
// de execução e heredoc, que precisam de um comportamento de sinal diferente.



void	define_signals(void)
{
    define_interactive_signals();
}