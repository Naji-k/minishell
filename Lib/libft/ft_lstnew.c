/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/21 13:06:06 by ysrondy       #+#    #+#                 */
/*   Updated: 2022/10/21 13:40:06 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	struct s_list	*new_list;

	new_list = (malloc)(sizeof(struct s_list));
	if (!new_list)
		return (0);
	new_list->content = content;
	new_list->next = NULL;
	return (new_list);
}

/*int main(void)
{
	printf("%p", ft_lstnew("Hello"));
}*/
