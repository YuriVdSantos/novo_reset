#include "minishell.h"


// void	handle_pipe(t_ctx ctx, int original_fd_out, char *current_command, t_cmd *commands)
// {
//     static int  pipe_fds[2];
//     int         is_first_command;
//     int         has_next_command;
//     t_cmd       *last_command = commands->next;

//     while (commands && commands->next)
//         commands = commands->next;
//     if(current_command == commands)
//         is_first_command = 0;

//     if (current_command != last_command)
//         has_next_command = 1;

//     if (!is_first_command)
//     {
//         redirect_fd(pipe_fds[IN], STDIN_FILENO);
//         close(pipe_fds[IN]);
//     }
//     if (has_next_command)
//     {
//         if (pipe(pipe_fds) == -1)
//         {
//             print_perror_msg("pipe", current_command);
//             return;
//         }
//         redirect_fd(pipe_fds[OUT], STDOUT_FILENO);
//         close(pipe_fds[OUT]);
//     }
//     else
//     {
//         redirect_fd(original_fd_out, STDOUT_FILENO);
//     }
// }