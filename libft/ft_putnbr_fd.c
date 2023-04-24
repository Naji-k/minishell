/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putnbr_fd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/14 11:56:24 by ysrondy       #+#    #+#                 */
/*   Updated: 2022/10/23 16:50:47 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>

void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (n >= 0 && n <= 9)
	{
		c = n + '0';
		write(fd, &c, 1);
	}
	else
	{
		if (n < 0)
		{
			if (n == -2147483648)
				write(fd, "-2147483648", 11);
			else
			{
				write(fd, "-", 1);
				ft_putnbr_fd((n * -1), fd);
			}
		}
		else
		{
			ft_putnbr_fd(n / 10, fd);
			ft_putnbr_fd(n % 10, fd);
		}
	}
}

/*int main(void)
{
	int n = -112341;
	int fd = 1;
	ft_putnbr_fd(n, fd);
}*/
