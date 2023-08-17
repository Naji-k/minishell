/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ftp_strdup.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/24 13:59:24 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/24 13:59:24 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ftp_strdup(char *s)
{
	size_t	len;
	char	*str;

	if (!s || s[0] == '\0')
		return (NULL);
	len = ft_strlen(s);
	str = ft_calloc((len + 1), sizeof(char));
	if (!str)
		return (NULL);
	ft_memcpy(str, s, len);
	free(s);
	return (str);
}
