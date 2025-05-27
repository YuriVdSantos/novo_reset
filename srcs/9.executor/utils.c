#include "minishell.h"

int	str_equal(const char *str1, const char *str2)
{
	size_t	size;

	if (!str1 || !str2)
		return (0);
	size = ft_strlen(str1);
	if (size != ft_strlen(str2))
		return (0);
	return (ft_strncmp(str1, str2, size) == 0);
}

void	move_one_forward(char *str)
{
	ft_memmove(str, str + 1, ft_strlen(str + 1) + 1);
}

int	is_empty(char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (*str != ' ')
			return (0);
		str++;
	}
	return (1);
}

void	print_error_msg(char *command, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	is_builtin(char *command)
{
	if (!command)
		return (0);
	if (str_equal(command, "echo"))
		return (1);
	if (str_equal(command, "cd"))
		return (1);
	if (str_equal(command, "pwd"))
		return (1);
	if (str_equal(command, "export"))
		return (1);
	if (str_equal(command, "unset"))
		return (1);
	if (str_equal(command, "env"))
		return (1);
	if (str_equal(command, "exit"))
		return (1);
	return (0);
}

// Por no diretorio redirects
void	redirect_fd(int fd_to_redirect, int fd_location)
{
    dup2(fd_to_redirect, fd_location);
    close(fd_to_redirect);
}

// Por no diretorio redirects
void	redirect_fds(int fd_in, int fd_out)
{
    if (fd_in != STDIN_FILENO)
        redirect_fd(fd_in, STDIN_FILENO);
    if (fd_out != STDOUT_FILENO)
        redirect_fd(fd_out, STDOUT_FILENO);
}

// Por no diretorio redirects
char	get_next_redirect(char *str)
{
    while (*str)
    {
        if (*str == '\'')
        {
            str++;
            while (*str != '\'')
                str++;
        }
        if (*str == '"')
        {
            str++;
            while (*str != '"')
                str++;
        }
        if (*str == '<' || *str == '>' || *str < 0)
            return (*str);
        str++;
    }
    return (0);
}

//Por no diretorio redirects
static char	*tmp_filename(int heredoc_number)
{
    char	filename[30];
    char	*number_str;

    ft_bzero(filename, 30);
    number_str = ft_itoa(heredoc_number);
    ft_strlcat(filename, "/tmp/heredoc", 30);
    ft_strlcat(filename, number_str, 30);
    free(number_str);
    return (ft_strdup(filename));
}

//Por no diretorio redirects
void	redirect_heredoc(char *command, int heredoc_number)
{
    char	*filename;
    int		tmp_file_fd;
    char	*heredoc_position;

    filename = tmp_filename(heredoc_number);
    tmp_file_fd = open(filename, O_RDONLY);
	if (tmp_file_fd == -1)
    {
        print_error_msg("open", filename);
        free(filename);
        return ;
    }
    free(filename);
    redirect_fd(tmp_file_fd, STDIN_FILENO);
    heredoc_position = get_redirect_position(command, heredoc_number);
    move_one_forward(heredoc_position);
}

// Por no diretorio redirects
int	redirect_output(char *command)
{
    char	*output_redirect;
    char	*file_name;
    int		fd;
    int		open_flags;

    output_redirect = get_redirect_position(command, '>');
    if (!output_redirect)
        return (SUCCESS);
    open_flags = (output_redirect[1] == '>') ? (O_WRONLY | O_CREAT | O_APPEND) : (O_WRONLY | O_CREAT | O_TRUNC);
    file_name = get_label_name(output_redirect);
    fd = open(file_name, open_flags, 0644);
    if (fd == -1)
    {
        print_error_msg("open", file_name);
        free(file_name);
        return (FAILED);
    }
    redirect_fd(fd, STDOUT_FILENO);
    free(file_name);
    return (SUCCESS);
}

// Por no diretorio redirects
int	redirect_input(char *command)
{
    char	*input_redirect;
    char	*file_name;
    int		fd;

    input_redirect = get_redirect_position(command, '<');
    if (!input_redirect)
        return (SUCCESS);
    file_name = get_label_name(input_redirect);
    fd = open(file_name, O_RDONLY, FD_CLOEXEC);
    if (fd == -1)
    {
        print_error_msg("open", file_name);
        free(file_name);
        return (FAILED);
    }
    redirect_fd(fd, STDIN_FILENO);
    free(file_name);
    return (SUCCESS);
}

// Por no diretorio redirects
char	*get_redirect_position(char *str, char redirect_char)
{
    while (*str)
    {
        if (*str == '\'')
        {
            str++;
            while (*str != '\'')
                str++;
        }
        if (*str == '"')
        {
            str++;
            while (*str != '"')
                str++;
        }
        if (*str == redirect_char)
            return (str);
        str++;
    }
    return (NULL);
}

int	is_name_delimeter(char c)
{
	if (c == ' ' || c == '>' || c == '<' || c == '|' || c == '\t')
		return (1);
	return (0);
}

static int	get_name_end(char *name)
{
	int	name_end;

	name_end = 0;
	while (name[name_end] && !is_name_delimeter(name[name_end]))
	{
		if (name[name_end] == '\'')
		{
			move_one_forward(&name[name_end]);
			while (name[name_end] && name[name_end] != '\'')
				name_end++;
			move_one_forward(&name[name_end]);
		}
		else if (name[name_end] == '"')
		{
			move_one_forward(&name[name_end]);
			while (name[name_end] && name[name_end] != '"')
				name_end++;
			move_one_forward(&name[name_end]);
		}
		else if (name[name_end] && !is_name_delimeter(name[name_end]))
			name_end++;
	}
	return (name_end);
}

char	*get_label_name(char *redirect_str)
{
	int		name_start;
	int		name_end;
	char	*name;
	char	*remaining_cmd;

	name_start = 0;
	move_one_forward(redirect_str);
	if (redirect_str[name_start] == '>')
		move_one_forward(&redirect_str[name_start]);
	while (redirect_str[name_start] == ' ' || redirect_str[name_start] == '\t')
		name_start++;
	name_end = get_name_end(&redirect_str[name_start]);
	name = ft_substr(&redirect_str[name_start], 0, name_end);
	remaining_cmd = &redirect_str[name_start + name_end];
	ft_memmove(redirect_str, remaining_cmd, ft_strlen(remaining_cmd) + 2);
	return (name);
}
