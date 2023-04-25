/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:32:29 by ysrondy           #+#    #+#             */
/*   Updated: 2022/10/24 10:34:10 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static void	free_everything(char **string, int i);
static int	find_strlen(char const *s, char c);
static int	get_nstrings(char const *s, char c);

char	**ft_split(char const *s, char c)
{
	size_t	j;
	size_t	i;
	size_t	len;
	char	**strings;

	i = -1;
	j = 0;
	len = get_nstrings(s, c);
	strings = (char **)malloc(sizeof(char *) * (len + 1));
	if (!strings)
		return (0);
	while (++i < len)
	{
		while (s[j] == c)
			j++;
		strings[i] = ft_substr(s, j, find_strlen(&s[j], c));
		if (strings[i] == NULL)
			free_everything(strings, i);
		while (s[j] != c && s[j] != '\0')
			j++;
	}
	strings[i] = NULL;
	return (strings);
}

static void	free_everything(char **string, int i)
{
	while (i >= 0)
	{
		free(string[i]);
		i--;
	}
	free(string);
}

static int	get_nstrings(char const *s, char c)
{
	size_t	count;
	size_t	i;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			i++;
		else
		{
			count++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
	}
	return (count);
}

static int	find_strlen(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (i);
		else
			i++;
	}
	return (i);
}

/*int main(void)
{
	char str[] = "hello,goodbye,doei";
	char **str2 = ft_split(str, ',');
	for (int i = 0; i < 3; i++)
		printf("String: %s\n", str2[i]);
}*/
