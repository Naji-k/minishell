/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 10:51:17 by ysrondy           #+#    #+#             */
/*   Updated: 2022/10/23 16:49:32 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;
	void	*bytes;

	i = 0;
	bytes = (unsigned char *)s;
	while (i < n)
	{
		if (((unsigned char *)bytes)[i] == (unsigned char)c)
			return (&bytes[i]);
		i++;
	}
	return (NULL);
}

/*int main(void)
{
	char str[] = "/|\x12\xff\x09\x42\2002\42|\\";
	char *ptr = memchr(str, '\200', sizeof(char) * 10);
	printf("%s\n", ptr);
	char *mptr = ft_memchr(str, '\200', sizeof(char) * 10);
	printf("%s\n", mptr);
}*/
