# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address -fPIC
IFLAGS = -I./Libft42 -I. -I/usr/include/readline
LDFLAGS = -L./Libft42 -lft -lreadline -no-pie

# Project Name
NAME = minishell

# Source Files
C_FILES = minishell.c \
		  srcs/0.init_and_clean_ctx/free_ctx.c \
		  srcs/0.init_and_clean_ctx/free_env_list.c \
		  srcs/0.init_and_clean_ctx/free_lists.c \
		  srcs/0.init_and_clean_ctx/init_ctx.c \
		  srcs/1.process_input/process_input.c \
		  srcs/12.handle_new_env/assignment_handler.c \
		  srcs/12.handle_new_env/create_new_env.c \
		  srcs/2.tokenizer/handle_env_var.c \
		  srcs/2.tokenizer/handle_pipe.c \
		  srcs/2.tokenizer/handle_quote.c \
		  srcs/2.tokenizer/handle_redir.c \
		  srcs/2.tokenizer/handle_word.c \
		  srcs/2.tokenizer/tokenizer.c \
		  srcs/3.syntax_validation/syntax_validation.c \
		  srcs/4.parser/handle_cmd.c \
		  srcs/4.parser/handle_cmd_1.c \
		  srcs/4.parser/parsing.c \
		  srcs/5.expander/quotes_expander.c \
		  srcs/5.expander/var_expander.c \
		  srcs/6.signals/signals.c \
		  srcs/7.memory_mgmt/safe_malloc.c \
		  srcs/8.error_mgmt/print_error.c \
		  srcs/9.executor/wait.c \
		  srcs/9.executor/split_args.c \
		  srcs/9.executor/pipes.c \
		  srcs/9.executor/one_command_utils.c \
		  srcs/9.executor/utils_multiple_cmd.c \
		  srcs/9.executor/utils.c \
		  srcs/9.executor/execute_builtin.c \
		  srcs/9.executor/utils_envp.c \
		  srcs/9.executor/execute_external.c \
		  srcs/9.executor/execute_multiple_commands.c \
		  srcs/9.executor/execute_one_command.c \
		  srcs/9.executor/get_path.c \
		  srcs/9.executor/multiple_commands_utils.c \
		  srcs/10.builtins/ft_cd.c \
		  srcs/10.builtins/ft_echo.c \
		  srcs/10.builtins/ft_env.c \
		  srcs/10.builtins/ft_exit.c \
		  srcs/10.builtins/ft_export.c \
		  srcs/10.builtins/ft_pwd.c \
		  srcs/10.builtins/ft_unset.c \
		  srcs/10.builtins/builtins_utils.c \
		  utils/main_utils.c \
		  utils/parsing_utils.c \
		  utils/safe_split.c \
		  utils/safe_utils_libft.c \
		  utils/safe_utils_libft_1.c \
		  utils/tokenizer_utils.c

# Object Files
OBJS = $(C_FILES:.c=.o)

# Libft
LIBFT_DIR = Libft42
LIBFT = $(LIBFT_DIR)/libft.a

# Default rule
all: $(NAME)

# Rule to build Libft
$(LIBFT):
	@echo "Making Libft..."
	@$(MAKE) -C $(LIBFT_DIR)

# Rule to build the main program
$(NAME): $(OBJS) $(LIBFT)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(NAME) compiled successfully!"

# Rule to compile .c files to .o files
# Adding minishell.h and libft.h as dependencies for .o files
%.o: %.c minishell.h $(LIBFT_DIR)/libft.h
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

# Clean rule
clean:
	@echo "Cleaning Libft object files..."
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "Cleaning project object files..."
	@rm -f $(OBJS)
	@echo "Object files cleaned!"

# Full clean rule
fclean: clean
	@echo "Cleaning Libft library..."
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "Cleaning $(NAME)..."
	@rm -f $(NAME)
	@echo "$(NAME) fully cleaned!"

# Rebuild rule
re: fclean all

# Phony targets prevent conflicts with files of the same name
.PHONY: all clean fclean re
