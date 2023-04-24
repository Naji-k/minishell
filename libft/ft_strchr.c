/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 10:01:01 by ysrondy           #+#    #+#             */
/*   Updated: 2022/10/23 16:51:28 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*word;

	word = (char *)s;
	i = 0;
	while (i < ft_strlen((char *)s) + 1)
	{
		if ((unsigned char)c == word[i])
			return (&word[i]);
		i++;
	}
	return (NULL);
}

/*int main(void)
{
	char string[] = "there is so \0ma\0ny \0 \\0 in t\0his stri\0ng !\0\0\0\0";
	char *string2 = strchr(string, '\0');
	printf("Og: %s\n", string2);
	
	char *string3 = ft_strchr(string, '\0');
	printf("Mine: %s\n", string3);
}*/
