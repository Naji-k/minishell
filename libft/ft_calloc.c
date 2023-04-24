/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 13:46:59 by ysrondy           #+#    #+#             */
/*   Updated: 2022/10/23 16:57:46 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*ptr;

	ptr = malloc(nmemb * size);
	if (ptr == NULL)
		return (NULL);
	if (size != 0 || nmemb != 0)
		ft_bzero(ptr, nmemb * size);
	return ((void *) ptr);
}

/*int main(void)
{
	int *ptr;
	int *ptr2;
	
	ptr = calloc(0,  0); 
	ptr2 = ft_calloc(0, 0);
	for (int i = 0; i < 5; i++)
		ptr[i] = i; 
	for (int j = 0; j < 5; j++)
		printf("Og: %d\n", ptr[j]);
	
	for (int i = 0; i < 5; i++)
		ptr2[i] = i; 
	for (int j = 0; j < 5; j++)
		printf("Mine: %d\n", ptr2[j]);
}*/
