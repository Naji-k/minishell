/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoll.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/28 19:59:35 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/08/28 19:59:35 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

int64_t	ft_atoll(char *str)
{
	uint64_t	res;
	int32_t		sign;
	int32_t		i;

	i = 0;
	res = 0;
	sign = 1;
	if (str[0] == '-' || str[0] == '+')
	{
		i++;
		if (str[0] == '-')
			sign = -1;
	}
	while (str[i])
	{
		res *= 10;
		res += str[i] - '0';
		if (res > __LONG_LONG_MAX__ && !(res - __LONG_LONG_MAX__ == 1
				&& sign == -1))
			return (error_exit(str), 255);
		i++;
	}
	return ((int64_t)res * sign);
}
