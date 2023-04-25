/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:06:38 by ysrondy           #+#    #+#             */
/*   Updated: 2022/10/06 18:25:27 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <string.h>
#include <stdio.h>

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (src[len] != '\0')
		len++;
	if (size == 0)
		return (len);
	while (src[i] != '\0' && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len);
}

/*int main(void)
{
	char src[5] = "hell";
	char dest[5];
	char msrc[5] = "hell";
	char mdest[5];

	printf("Return Og: %lu\n", strlcpy(dest, src, 0)); 
	printf("Return Mine: %lu\n", ft_strlcpy(mdest, msrc, 0));

	printf("String Og: %s\n", dest); 
	printf("String Mine: %s\n", mdest);
}*/
