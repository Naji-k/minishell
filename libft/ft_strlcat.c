/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:25:45 by ysrondy           #+#    #+#             */
/*   Updated: 2022/10/23 16:52:43 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dst[i] && i < size)
		i++;
	while (src[j] && (i + j + 1) < size)
	{
		dst[i + j] = src[j];
		j++;
	}
	if (i < size)
		dst[i + j] = '\0';
	return (i + ft_strlen(src));
}

/*int main(void)
{
	//char src[] = "pqrstuvwxyz";
	//char dest[] = "abcd";
	
	char src2[] = "pqrstuvwxyz";
	char dest2[] = "abcd";
	
	//printf("Return Og: %lu\n", strlcat(dest, src, 20));
	printf("Return Mine: %lu\n", ft_strlcat(dest2, src2, 20));
	
	//printf("String Og: %s\n", dest);
	printf("String Mine: %s\n", dest2);
}*/
