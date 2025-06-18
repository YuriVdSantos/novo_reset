

#include "../../minishell.h"



static void	handle_execve_errors(char **args, const char *path, t_ctx *ctx) 
{
	(void)path; 
	
	
	print_error(ctx, args[0], errno, ctx->exit_status);
	close_all_fds();
	exit(ctx->exit_status); 
}



int	execute_external(char **args, t_env *minienv, t_ctx *ctx)
{
	const char	*path;
	char		*command;
	char		**envp;

	(void)minienv;
	command = args[0];

	if (is_empty(command))
	{
		close_all_fds();
		exit(EXIT_SUCCESS);
	}

	path = get_path(command, ctx); 
	if (path == NULL) 
	{
		
		
		handle_execve_errors(args, path, ctx);
	}

	rl_clear_history(); 
	close_extra_fds();  

	envp = ctx->env_list_str; 

	if (execve(path, args, envp) == -1)
	{
		
		
		if (errno == ENOENT)
			ctx->exit_status = 127;
		else if (errno == EACCES)
			ctx->exit_status = 126;
		else 
			ctx->exit_status = EXIT_FAILURE;

		handle_execve_errors(args, path, ctx); 
	}

	
	exit(EXIT_FAILURE);
}