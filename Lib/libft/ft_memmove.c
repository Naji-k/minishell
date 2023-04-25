/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 09:35:56 by ysrondy           #+#    #+#             */
/*   Updated: 2022/10/23 16:50:18 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;

	i = 0;
	if (!dst && !src)
		return (0);
	if (src > dst)
	{
		while (i < len)
		{
			((unsigned char *) dst)[i] = ((unsigned char *) src)[i];
			i++;
		}
	}
	else
	{
		while (len)
		{
			((unsigned char *) dst)[len - 1] = ((unsigned char *) src)[len - 1];
			len--;
		}
	}
	return (dst);
}

/*int	main(void)
{
	int	arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int	dest[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	int	marr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int	mdest[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	memmove(arr+2, arr, sizeof(int)*8);	
	for (int i = 0; i < 10; i++)
		printf("Og: %d\n", arr[i]); 

	ft_memmove(marr+2, marr, sizeof(int)*8);
	for (int i = 0; i < 10; i++)
		printf("My Func: %d\n", marr[i]);
}*/
