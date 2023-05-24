/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ftp_substr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:08:43 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/04/11 17:08:43 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ftp_substr(char *s, int start, int len)
{
	char	*sub_str;
	int		str_len;
	int		s_len;

	s_len = ft_strlen(s);
	if (start >= s_len || !*s || s[start] == '\0')
	{
		sub_str = malloc(sizeof(char));
		if (!sub_str)
			return (NULL);
		sub_str[0] = '\0';
		return (free(s), sub_str);
	}
	if (start + len > s_len)
		str_len = s_len - start;
	else
		str_len = len;
	sub_str = malloc(sizeof(char) * (str_len + 1));
	if (!sub_str)
		return (free(s), NULL);
	ft_strlcpy(sub_str, (s + start), str_len + 1);
	return (free(s), sub_str);
}
