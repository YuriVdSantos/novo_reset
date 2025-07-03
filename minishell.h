/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yvieira- <yvieira-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:14:01 by jhualves          #+#    #+#             */
/*   Updated: 2025/07/02 20:49:20 by yvieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft42/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <limits.h>
# include <sys/fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <stdbool.h>
# include <sys/stat.h>
# include <signal.h>
# include <sys/types.h>

# define ERR_SUCCESS_MSG "Success"
# define ERR_GENERAL_MSG "General error"
# define ERR_MISUSE_SHELL_MSG "Misuse of shell builtins"
# define ERR_CANT_EXECUTE_MSG "Command invoked cannot execute"
# define ERR_CMD_NOT_FOUND_MSG "command not found"
# define ERR_EXIT_ARG_MSG "Invalid argument to exit"
# define ERR_CTRL_C_MSG "Script terminated by Ctrl-C"
# define ERR_KILL_9_MSG "Script terminated by kill -9"
# define ERR_SEGFAULT_MSG "Segmentation fault"
# define ERR_EXIT_RANGE_MSG "Exit status out of range"
# define ERR_UNKNOWN_MSG "Unknown error"
# define _POSIX_C_SOURCE 200809L
# define ERR_PERMISSION_DENIED "Permission denied"
# define NOT_EXECUTABLE_MSG "Is a directory"
//Para Execução
# define INTERRUPT 128
# define FORK_ERROR -1
# define TRUE 1
# define FALSE 0
# define IN 0
# define OUT 1
# define SUCCESS   1
# define FAILED    0
# define NO_REDIRECT -1
# define CMD_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define NOT_EXECUTABLE 126
# define OUT_OF_RANGE 255
# define BUILTIN_MISUSE 2
# define CMD_NOT_FOUND_MSG	"command not found"
# define EXPAND_BUFFER_SIZE 4096

typedef struct s_expand_buffer
{
	char	*result;
	char	*cursor;
	size_t	remaining;
}	t_expand_buffer;

typedef enum e_alloc_type
{
	ALLOC_TYPE_GENERIC,
	ALLOC_TYPE_CMD,
	ALLOC_TYPE_TOKEN,
	ALLOC_TYPE_REDIR,
	ALLOC_TYPE_STR,
	ALLOC_TYPE_STRING,
	ALLOC_TYPE_ENV_NODE,
	ALLOC_TYPE_CTX
}	t_alloc_type;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	SQUOTE,
	DQUOTE,
	ENV_VAR,
	ASSIGNMENT_VAR,
	END,
	PASS,
	ERROR
}	t_token_type;

typedef enum e_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_HEREDOC,
	REDIR_APPEND
}	t_redir_type;

typedef enum e_cmd_type
{
	CMD_BUILTIN,
	CMD_EXTERNAL
}	t_cmd_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	int				fd;
	bool			was_heredoc;
	struct s_redir	*next;
}	t_redir;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	char			*cmd_path;
	int				pipe[2];
	t_redir			*redirections;
	t_cmd_type		type;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_allocation
{
	void				*ptr;
	t_alloc_type		type;
	struct s_allocation	*next;
}	t_allocation;

typedef struct s_ctx
{
	t_env					*env_list;
	t_cmd					*cmd_list;
	t_token					*token_list;
	char					*input;
	int						exit_status;
	int						previous_exit_status;
	int						current_exit_status;
	char					**env_list_str;
	char					*last_error_message;
	t_allocation			*allocations;
	bool					is_interactive;
	char					*pwd;
	char					*oldpwd;
}	t_ctx;

extern volatile sig_atomic_t	g_signal;

// Protótipos de funções
// =============================================================================
// srcs/0.init_and clean_ctx/
// =============================================================================

// srcs/0.init_and_clean_ctx/free_ctx.c
void	free_context(t_ctx *ctx);
void	free_all_allocations(t_ctx *ctx);
void	super_free(t_ctx *ctx);
void	safe_free(t_ctx *ctx, void *ptr);

// srcs/0.init_and_clean_ctx/free_lists.c
void	free_redir_list(t_redir *redir);
void	free_cmd_list(t_cmd *cmd);
void	free_token_list(t_token *token);
void	free_string_array(char **array);
void	free_env_list(t_env *env_list); // Added prototype

// srcs/0.init_and_clean_ctx/init_ctx.c
t_ctx	*init_ctx(t_ctx *ctx, char **env);
void	init_environment(t_ctx *ctx, char **env_array);
void	add_env_node(t_ctx *ctx, t_env **list_head, const char *env_var);

// =============================================================================
// srcs/1.process_input/
// =============================================================================

// srcs/1.process_input/process_input.c
void	process_input(t_ctx *ctx, const char *input);

// =============================================================================
// srcs/2.tokenizer/
// =============================================================================

// srcs/2.tokenizer/handle_env_var.c
void	token_handle_env_var(t_ctx *ctx, const char **input, t_token **tokens);

// srcs/2.tokenizer/handle_pipe.c
void	token_handle_pipe(t_ctx *ctx, const char **input, t_token **tokens);

// srcs/2.tokenizer/handle_quote.c
void	token_handle_quote(t_ctx *ctx, const char **input, t_token **tokens);

// srcs/2.tokenizer/handle_redir.c
void	token_handle_redir(t_ctx *ctx, const char **input, t_token **tokens);

// srcs/2.tokenizer/handle_word.c
void	token_handle_word(t_ctx *ctx, const char **input, t_token **tokens);
int		is_valid_var_char(char c);

// srcs/2.tokenizer/tokenizer.c
t_token	*tokenize_input(t_ctx *ctx, const char **input);
void	ft_lstadd_back(t_token **head, t_token *new_node);
t_token	*new_token(t_ctx *ctx, t_token_type type, const char *str);

// =============================================================================
// srcs/3.syntax_validation/
// =============================================================================

// srcs/3.syntax_validation/syntax_validation.c
bool	validate_syntax(t_ctx *ctx, t_token *tokens);
bool	check_pipes(t_ctx *ctx, t_token *tokens);
bool	check_redirections(t_ctx *ctx, t_token *tokens);
bool	check_initial_pipe(t_ctx *ctx, t_token *tokens);
bool	check_consecutive_pipes(t_ctx *ctx, t_token *prev, t_token *current);
bool	check_redirection_syntax(t_ctx *ctx, t_token *current);
bool	check_final_pipe(t_ctx *ctx, t_token *current);
bool	is_valid_filename_token(t_token_type type);

// =============================================================================
// srcs/4.parser/
// =============================================================================

// srcs/4.parser/handle_cmd.c
void	handle_pipe(t_token **tmp, t_cmd **current);
void	handle_redir(t_ctx *ctx, t_token **tmp, t_cmd *current);
void	handle_word(t_ctx *ctx, t_token **tmp, t_cmd *current);
void	handle_dquote(t_ctx *ctx, t_token **tmp, t_cmd **current);
void	handle_squote(t_ctx *ctx, t_token **tmp, t_cmd **current);

// srcs/4.parser/handle_cmd_1.c
void	handle_assignment_var(t_ctx *ctx, t_token **tmp, t_cmd *current);
void	handle_env_var(t_ctx *ctx, t_token **tmp, t_cmd *current);
void	handle_parse_error(t_ctx *ctx, t_token **tmp);

// srcs/4.parser/parsing.c
t_cmd	*parse_tokens(t_ctx *ctx, t_token **tokens);
int		only_var_assignments(t_token *tokens);
void	set_type_word(t_token *tokens);
char	*remove_quotes(char *str);

// =============================================================================
// srcs/5.expander/
// =============================================================================

// srcs/5.expander/quotes_expander.c
char	*expand_dquotes(t_ctx *ctx, const char *input);
char	*get_env_value(t_ctx *ctx, const char *key);
int		var_name_length(const char *input);
char	*expand_env_var(t_ctx *ctx, const char *input, int *len);

// srcs/5.expander/string_expander.c
int		is_valid_dollar(char c);
char	*expand_string(t_ctx *ctx, const char *input);

// =============================================================================
// srcs/6.signals/
// =============================================================================
void	define_execute_signals(int child_pid);
void	define_signals(void);
void	define_heredoc_signals(int child_pid);
void	define_interactive_signals(void);
void	define_non_interactive_signals(void);

// =============================================================================
// srcs/7.memory_mgmt/
// =============================================================================

// srcs/7.memory_mgmt/safe_malloc.c
void	*safe_malloc(t_ctx *ctx, size_t size, t_alloc_type type);
void	safe_free_all(t_ctx *ctx);
void	*safe_realloc(t_ctx *ctx, void *ptr, size_t new_size, \
		t_alloc_type type);

// =============================================================================
// srcs/8.error_mgmt/
// =============================================================================

// srcs/8.error_mgmt/print_error.c
char	*get_error_message(int error_code);
bool	handle_error(t_ctx *ctx, char *msg, int errnum, int exit_status);
bool	syntax_error(t_ctx *ctx, char *msg);
void	print_error(t_ctx *ctx, char *msg, int errnum, int exit_status);

// =============================================================================
// srcs/9.executor/
// =============================================================================
// executes
int		execute_multiple_commands(t_cmd *commands, t_ctx *ctx);
int		execute_one_command(t_cmd *command, t_ctx *ctx);

// one command utils
void	save_original_fd_in(int original_fds[2]);
int		handle_input_redirect(char *command, int original_fds[2]);
int		handle_output_redirect(char *command, int original_fds[2]);

// multiple commands utils
int		*init_children_pid(char **commands);
void	clean_after_execute(int *children_pid);
void	quit_child(char **commands, t_ctx *ctx);

// execute builtin
int		execute_forked_builtin(char **args, t_ctx *ctx);
int		execute_builtin(char **args, t_ctx *ctx);
// execute external
int		execute_external(char **args, t_env *minienv, t_ctx *ctx);
char	*get_path(char *command, t_ctx *ctx);

// wait after fork
int		wait_for_child(int child_pid, int is_last_child, t_ctx *ctx);
int		wait_for_children(int children_pid[1024], t_ctx *ctx);

// redirects
int		redirect_fd(int fd_to_redirect, int fd_location);
void	redirect_fds(int fd_in, int fd_out);
char	get_next_redirect(char *cmd);
void	close_all_fds(void);
void	close_extra_fds(void);
char	*get_label_name(char *redirect_str);
void	redirect_heredoc(char *command, int heredoc_number);
char	*get_redirect_position(char *str, char redirect_char);
char	*get_label_name(char *redirect_str);
int		is_name_delimeter(char c);

//utils
int		is_empty(char *str);
char	**minienv_to_envp(t_env *minienv);
void	print_error_msg(char *command, char *msg);
int		is_builtin(char *command);
int		arr_len(char **arr);
void	close_all_fds(void);
void	close_extra_fds(void);
int		redirect_output(char *command);
int		redirect_input(char *command);
t_env	*minienv_node(char *name, t_env *minienv);
void	free_array(char **arr);
void	sync_env_list_str(t_ctx *ctx);
size_t	minienv_size(t_env *minienv);
int		is_quote(char c);

// =============================================================================
// srcs/10.builtins/
// =============================================================================

int		ft_unset(char **args, t_ctx *ctx);
int		ft_pwd(void);
int		ft_export(char **args, t_ctx *ctx);
int		fits_in_long_long(char *str);
int		ft_exit(char **args, t_ctx *ctx);
int		ft_env(t_ctx *ctx);
int		is_only_n(const char *str);
int		ft_echo(char **args, t_ctx *ctx);
int		ft_cd(char **args, t_ctx *ctx);
int		cd_error(void);
int		is_valid_env_identifier(const char *name);

//utils builtins
void	move_one_forward(char *str);
int		str_equal(const char *str1, const char *str2);
void	minienv_update(char *name, char *value, t_env *minienv);
void	list_append(char *key_pair, t_env **list);
int		is_valid_varname(char *name);
char	*value_only(char *key_pair);
char	*name_only(char *key_pair);
char	*create_keypair(char *name, char *value);

// =============================================================================
// srcs/11.redirects/
// =============================================================================

int		apply_redirections(t_cmd *cmd, int original_fds[2]);
void	restore_original_fds(int original_fds[2]);
void	save_original_fd_in(int original_fds[2]);
void	save_original_fd_out(int original_fds[2]);
int		process_heredocs(t_cmd *cmd_list, t_ctx *ctx);
void	cleanup_heredocs(t_cmd *cmd_list);

// =============================================================================
// srcs/12.handle_new_env/
// =============================================================================

// srcs/12.handle_new_env/create_new_env.c
t_env	*find_env_var(t_env *env_list, const char *key);
void	set_env_var(t_ctx *ctx, const char *assignment);
void	add_new_env_var(t_ctx *ctx, char *key, char *value);
void	update_existing_var(t_ctx *ctx, t_env *var, const char *value);
void	unset_env_var(t_ctx *ctx, const char *key);

// srcs/12.handle_new_env/assignment_handler.c
void	process_assignments(t_ctx *ctx, t_cmd *cmd);
void	unset_string_env_var(t_ctx *ctx, const char *key);

// =============================================================================
// utils/
// =============================================================================
//utils/execute_utils.c
void	exit_with_error(t_ctx *ctx, const char *arg, \
	const char *msg, int code);

// utils/main_utils.c
void	no_input(void);
void	input_null(t_ctx *ctx, char **input);
char	**dup_mtz(char **mtz);

// utils/parsing_utils.c
void	add_arg(t_ctx *ctx, t_cmd *cmd, char *value);
void	add_redir(t_cmd *cmd, t_redir_type type, char *file);
t_cmd	*new_cmd(void);

// utils/safe_split.c
char	**safe_split(t_ctx *ctx, const char *s, char c);

// utils/safe_utils_libft.c
char	*safe_strdup(t_ctx *ctx, const char *s);
char	*ft_safe_strndup(t_ctx *ctx, const char *s, size_t size);
char	*safe_substr(t_ctx *ctx, char const *s, unsigned int start, size_t len);
char	*safe_strchr(t_ctx *ctx, const char *s, int c);
char	*safe_strrchr(t_ctx *ctx, const char *s, int c);
char	*safe_strnstr(t_ctx *ctx, const char *haystack, \
		const char *needle, size_t len);
char	*safe_strtrim(t_ctx *ctx, char const *s1, char const *set);
char	*safe_strjoin(t_ctx *ctx, char const *s1, char const *s2);
char	*safe_itoa(t_ctx *ctx, long n);
int		ft_isalpha_upper(int c);
void	register_alloc(t_ctx *ctx, void *ptr, t_alloc_type type);

// utils/tokenizer_utils.c
int		define_substring(t_ctx *ctx, char **str, const char **input, \
		t_token_type type);
void	get_pid_var(t_ctx *ctx, char **str);
char	*get_var_value(t_ctx *ctx, const char *var_name, int *len);

// =============================================================================
// minishell.c (main program file)
// =============================================================================
int		main(int argc, char **argv, char **env);
t_ctx	*main_loop(t_ctx *ctx);

#endif
