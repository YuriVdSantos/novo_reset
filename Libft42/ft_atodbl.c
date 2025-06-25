/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atodbl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 16:32:33 by jhualves          #+#    #+#             */
/*   Updated: 2025/02/12 16:33:38 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long	ft_atodbl(char *str)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	// 1. Pular espaços em branco
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	// 2. Lidar com o sinal de + ou -
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	// 3. Converter os dígitos para número
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}
