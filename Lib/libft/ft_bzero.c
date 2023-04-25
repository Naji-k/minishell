/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_bzero.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 13:16:08 by ysrondy       #+#    #+#                 */
/*   Updated: 2022/10/23 16:48:13 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((char *)s)[i] = '\0';
		i++;
	}
}

/*int main(void)
{
	char buffer[5] = "hello";
	char mbuffer[5] = "hello";

	bzero(buffer, 2);
	printf("Og: %s\n", buffer);
	
	ft_bzero(mbuffer, 2);
	printf("Mine: %s\n", buffer);
}*/
