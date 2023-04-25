/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strmapi.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 09:48:06 by ysrondy       #+#    #+#                 */
/*   Updated: 2022/10/23 16:53:55 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	i;
	size_t	len;
	char	*str;

	i = 0;
	len = ft_strlen(s);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	while (i < len)
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*char function(unsigned int i, char c)
{
	char str;
	str = c - 32;
	return (str);
}

int main(void)
{
	char str[] = "hello";
	char *str2;
	str2 = ft_strmapi(str, *function);
	printf("%s\n", str2);
}*/
