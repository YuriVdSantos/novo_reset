// Arquivo: debug.h

#ifndef DEBUG_H
# define DEBUG_H

/* * A "mágica" acontece aqui. Se compilarmos com a flag -DDEBUG,
 * este cabeçalho ativará nossa ferramenta de depuração.
 * Caso contrário, ele não fará nada.
*/
# ifdef DEBUG
#  include <stdio.h> // Para usar __FILE__ e __LINE__

// 1. O protótipo da nossa função de depuração
void	my_free(void *ptr, const char *file, int line);
void	*my_malloc(size_t size, const char *file, int line);

// 2. A macro que substitui todas as chamadas de free e malloc
#  define free(ptr) my_free(ptr, __FILE__, __LINE__)
#  define malloc(size) my_malloc(size, __FILE__, __LINE__)

# endif // Fim do bloco #ifdef DEBUG

#endif // Fim do bloco #ifndef DEBUG_H