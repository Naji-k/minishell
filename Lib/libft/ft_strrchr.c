/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 10:28:06 by ysrondy           #+#    #+#             */
/*   Updated: 2022/10/23 16:54:54 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
char	*ft_strrchr(const char *s, int c)
{
	int		i;
	int		j;
	char	a;

	a = c;
	i = 0;
	j = -1;
	while (s[i])
	{
		if (s[i] == a)
			j = i;
		i++;
	}
	if (s[i] == a)
		j = i;
	if (j != -1)
		return ((char *) &s[j]);
	return (0);
}

/*int main(void)
{
	char string[] = "teste";
	
	char *string2 = strrchr(string, '\0');
	char *string3 = ft_strrchr(string, '\0');

	//printf("Og C: %d\n", 't' + 256);
	printf("pointer 0: %p\n", string+5);
	printf("Og: %p\n", string2);
	printf("Mine: %p\n", string3);
}*/
