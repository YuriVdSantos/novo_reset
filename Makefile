# Nome do executável final
NAME = minishell

# Compilador e flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# --- Diretórios ---
SRCS_DIR = srcs
OBJS_DIR = objs
LIBFT_DIR = Libft42
LIBFT = $(LIBFT_DIR)/libft.a

# --- Configuração da Biblioteca Readline ---
# Caminho padrão para Homebrew em Macs com Apple Silicon (arm64)
# Se o seu caminho for diferente, ajuste esta variável.
READLINE_PATH = /opt/homebrew/opt/readline

# Flags para o compilador (includes) e linker (libs)
IFLAGS = -I . -I $(LIBFT_DIR) -I$(READLINE_PATH)/include
LFLAGS = -L$(READLINE_PATH)/lib -lreadline

# --- Arquivos Fonte (.c) ---
SRCS =  minishell.c \
	srcs/0.init_and_clean_ctx/free_ctx.c \
	srcs/0.init_and_clean_ctx/free_env_list.c \
	srcs/0.init_and_clean_ctx/free_lists.c \
	srcs/0.init_and_clean_ctx/init_ctx.c \
	\
	srcs/1.process_input/process_input.c \
	\
	srcs/2.tokenizer/tokenizer.c \
	srcs/2.tokenizer/handle_env_var.c \
	srcs/2.tokenizer/handle_pipe.c \
	srcs/2.tokenizer/handle_quote.c \
	srcs/2.tokenizer/handle_redir.c \
	srcs/2.tokenizer/handle_word.c \
	\
	srcs/3.syntax_validation/syntax_validation.c \
	\
	srcs/4.parser/parsing.c \
		  srcs/3.syntax_validation/syntax_validation_utils.c \
	srcs/4.parser/handle_cmd.c \
	srcs/4.parser/handle_cmd_1.c \
	\
	srcs/5.expander/quotes_expander.c \
	srcs/5.expander/string_expander.c\
	\
	srcs/6.signals/signals.c \
	\
	srcs/7.memory_mgmt/safe_malloc.c \
	\
	srcs/8.error_mgmt/print_error.c \
	\
	srcs/9.executor/execute_one_command.c \
	srcs/9.executor/execute_multiple_commands.c \
	srcs/9.executor/execute_external.c \
	srcs/9.executor/execute_builtin.c \
	srcs/9.executor/wait.c \
	srcs/9.executor/get_path.c \
	srcs/9.executor/pipes.c \
	srcs/9.executor/multiple_commands_utils.c \
	srcs/9.executor/utils.c \
	srcs/9.executor/utils_envp.c \
	srcs/9.executor/utils_multiple_cmd.c \
	srcs/9.executor/split_args.c \
	\
	srcs/10.builtins/ft_cd.c \
	srcs/10.builtins/ft_echo.c \
	srcs/10.builtins/ft_env.c \
	srcs/10.builtins/ft_exit.c \
	srcs/10.builtins/ft_export.c \
	srcs/10.builtins/ft_pwd.c \
	srcs/10.builtins/ft_unset.c \
	srcs/10.builtins/builtins_utils.c \
	\
	srcs/11.redirects/redirects.c \
	srcs/11.redirects/redirect_utils.c \
	srcs/11.redirects/here_doc.c \
	\
	srcs/12.handle_new_env/assignment_handler.c \
	srcs/12.handle_new_env/create_new_env.c \
	\
	utils/main_utils.c \
	utils/parsing_utils.c \
	utils/safe_split.c \
	utils/safe_utils_libft.c \
	utils/safe_utils_libft_1.c \
	utils/tokenizer_utils.c

# --- Arquivos Objeto (.o) ---
OBJS = $(patsubst %.c, $(OBJS_DIR)/%.o, $(SRCS))

# Cores para a saída
GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m

# --- Regras Principais ---

all: $(NAME)

# Rule to build Libft
$(LIBFT):
	@echo "Making Libft..."
	@$(MAKE) -C $(LIBFT_DIR)

# Rule to build the main program
$(NAME): $(OBJS) $(LIBFT)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LFLAGS) -o $(NAME)
	@echo "$(GREEN)$(NAME) successfully compiled!$(RESET)"

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

leaks: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=readline.supp ./${NAME}


# --- Regras de Limpeza ---

clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	@rm -rf $(OBJS_DIR)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(YELLOW)Cleaning executable...$(RESET)"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

# Target para teste com flags de debug (-g)
debug: re

.PHONY: debug