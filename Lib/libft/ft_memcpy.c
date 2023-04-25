/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 13:23:09 by ysrondy       #+#    #+#                 */
/*   Updated: 2022/10/23 16:50:04 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (0);
	while (i < n)
	{
		((char *)dest)[i] = ((char *)src)[i];
		i++;
	}
	return (dest);
}

/*int main(void)
{
	char str[5] = "hello";
	char mstr[5] = "hello";
	char dest[5];
	char mdest[5];
	memcpy(str+2, str, 3);
	printf("Og: %s\n", dest);
	ft_memcpy(mstr+2, mstr, 3);
	printf("Mine: %s\n", mdest);
}*/
