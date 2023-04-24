/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_striteri.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 10:34:47 by ysrondy       #+#    #+#                 */
/*   Updated: 2022/10/23 16:56:17 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	size_t	i;

	i = 0;
	if (!s || !f)
		return ;
	while (s[i] != '\0')
	{
		f(i, (&s[i]));
		i++;
	}
}

/*void function(unsigned int i, char *c)
{
	i = 0;
	if (*c == 'e')
		*c = '?';
}

int main(void)
{
	char str[] = "hello";
	ft_striteri(str, function);
	printf("%s\n", str);
}*/
