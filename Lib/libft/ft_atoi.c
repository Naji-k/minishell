/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/07 14:00:58 by ysrondy       #+#    #+#                 */
/*   Updated: 2022/10/23 16:47:59 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	i;
	int	sum;

	i = 0;
	sum = 0;
	sign = 1;
	while (nptr[i] == '\f' || nptr[i] == ' ' || nptr[i] == '\n'
		|| nptr[i] == '\r' || nptr[i] == '\t' || nptr[i] == '\v')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9' && nptr[i] != '\0')
	{
		sum *= 10;
		sum += nptr[i++] - '0';
	}
	return (sum * sign);
}

/*int main(void)
{
	char str[] = "\x0e15";
	
	printf("Og: %d\n", atoi(str));
	printf("Mine: %d\n", ft_atoi(str));
}*/
