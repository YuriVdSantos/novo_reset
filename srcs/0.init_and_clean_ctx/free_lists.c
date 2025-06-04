
#include "minishell.h"

void	free_redir_list(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->filename);
		free(redir);
		redir = next;
	}
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*next;

	while (cmd)
	{
		next = cmd->next;
		free(cmd->args);
		free(cmd->cmd_path);
		free_redir_list(cmd->redirections);
		free(cmd->pipe);
		free(cmd);
		cmd = next;
	}
}

void	free_token_list(t_token *token)
{
	t_token	*next;

	while (token)
	{
		next = token->next;
		free(token->value);
		free(token);
		token = next;
	}
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		if (array[i] == NULL)
			return ;
		free(array[i]);
		i++;
	}
	free(array);
}