#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

static void	handle_sigint(int sig)
{
	(void)sig;
	g_signal = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	define_interactive_signals(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
}

void	define_non_interactive_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	define_execute_signals(int child_pid)
{
    if (child_pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
    }
    else
    {
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
    }
}

void	define_heredoc_signals(int child_pid)
{
    if (child_pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_IGN);
    }
    else
    {
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
    }
}

void	define_signals(void)
{
    define_interactive_signals();
}
