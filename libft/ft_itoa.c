/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 16:59:48 by ysrondy           #+#    #+#             */
/*   Updated: 2022/10/23 16:48:44 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static char	*ret_positive(int len, int num, int i, int n);
static char	*ret_negative(int len, int num, int i, int n);

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	int		num;
	int		i;

	i = 0;
	len = 0;
	num = n;
	if (n >= 0)
		str = ret_positive(len, num, i, n);
	else if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	else
		str = ret_negative(len, (num * -1), i, (n * -1));
	return (str);
}

static char	*ret_negative(int len, int num, int i, int n)
{
	char	*str;

	while (n > 0)
	{
		n /= 10;
		len++;
	}
	str = (char *)malloc(sizeof(char) * (len + 2));
	if (str == NULL)
		return (str);
	str[len + 1] = '\0';
	str[0] = '-';
	while ((i + 1) <= len)
	{
		if (num < 9)
		{
			str[len] = num + '0';
			break ;
		}
		str[len] = ((num % 10) + '0');
		num /= 10;
		len--;
	}
	return (str);
}

static char	*ret_positive(int len, int num, int i, int n)
{
	char	*str;

	while (n > 0)
	{
		n /= 10;
		len++;
	}
	if (num == 0)
		len++;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (str);
	str[len] = '\0';
	while (i < len)
	{
		len--;
		if (num < 9)
		{
			str[len] = num + '0';
			break ;
		}
		str[len] = ((num % 10) + '0');
		num /= 10;
	}
	return (str);
}

/*int main(void)
{
	int n;
	
	n = -214748;
	printf("Itoa: %s", ft_itoa(n));
}*/
