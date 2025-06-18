// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   handle_cmd_1.c                                     :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/05/25 23:23:43 by jhualves          #+#    #+#             */
// /*   Updated: 2025/06/15 20:23:04 by jhualves         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// void	handle_assignment_var(t_ctx *ctx, t_token **tmp, t_cmd *current)
// {
// 	char	*value;
// 	int		i;

// 	i = 0;
// 	while ((*tmp)->value && (*tmp)->value[i] && (*tmp)->value[i] != '=')
// 		i++;
// 	value = ft_strdup((*tmp)->value + i + 1);
// 	add_arg(current, value);
// 	set_env_var(ctx, (*tmp)->value);
// 	*tmp = (*tmp)->next;
// }

// void	handle_env_var(t_ctx *ctx, t_token **tmp, t_cmd *current)
// {
// 	char	*env_value;

// 	//se for unset tem que mandar a key não o o valor
// 	env_value = safe_strdup(ctx, get_env_value(ctx, (*tmp)->value));
// 	add_arg(current, env_value);
// 	current->type = CMD_BUILTIN;
// 	*tmp = (*tmp)->next;
// }

// void	handle_parse_error(t_ctx *ctx, t_token **tmp)
// {
// 	print_error(ctx, "syntax error", -1, 2);
// 	*tmp = (*tmp)->next;
// }


/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* handle_cmd_1.c                                     :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/05/25 23:23:43 by jhualves          #+#    #+#             */
/* Updated: 2025/06/15 20:23:04 by jhualves         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

void	handle_assignment_var(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	char	*value;
	int		i;

	i = 0;
	while ((*tmp)->value && (*tmp)->value[i] && (*tmp)->value[i] != '=')
		i++;
	value = ft_strdup((*tmp)->value + i + 1);
	add_arg(current, value);
	set_env_var(ctx, (*tmp)->value);
	*tmp = (*tmp)->next;
}

void	handle_env_var(t_ctx *ctx, t_token **tmp, t_cmd *current)
{
	char	*env_value;

    // Obtenha o valor da variável de ambiente.
    // get_env_value retorna NULL se a variável não for encontrada ou é vazia.
	env_value = get_env_value(ctx, (*tmp)->value);

    // Se a variável expande para uma string vazia ou não existe, NÃO adicione como argumento.
    // Isso simula o comportamento do bash onde argumentos vazios de expansões não-citadas são removidos.
    if (env_value == NULL || ft_strlen(env_value) == 0)
    {
        // Se env_value não é NULL, significa que get_env_value retornou um ponteiro para uma string vazia da lista de env.
        // Não é necessário liberar env_value aqui, pois ele é um ponteiro para a memória do ctx->env_list.
        // A string vazia seria liberada com o free_env_list.
        *tmp = (*tmp)->next; // Avança para o próximo token
        return; // Não adiciona o argumento
    }

    // Se env_value não é NULL e não é vazio, duplique-o e adicione como argumento.
    // Use safe_strdup para garantir que a memória seja rastreada pelo ctx.
	add_arg(current, safe_strdup(ctx, env_value));

	current->type = CMD_BUILTIN; // Isso é questionável, nem sempre um comando com var de ambiente é builtin. Mas vamos manter o foco no problema atual.
	*tmp = (*tmp)->next;
}

void	handle_parse_error(t_ctx *ctx, t_token **tmp)
{
	print_error(ctx, "syntax error", -1, 2);
	*tmp = (*tmp)->next;
}