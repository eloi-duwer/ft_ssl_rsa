/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_rand.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 02:53:12 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/26 23:51:48 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

void	get_rand(void *buff, size_t size)
{
	static int	fd = -1;

	if (fd == -1 && (fd = open("/dev/urandom", O_RDONLY)) == -1)
		exit(print_errno("ft_ssl: open /dev/urandom"));
	if (read(fd, buff, size) == -1)
		exit(print_errno("ft_ssl: read /dev/urandom"));
}

uint64_t	get_64b_rand(void)
{
	uint64_t rand;

	get_rand(&rand, sizeof(rand));
	return (rand);
}

uint64_t	get_rand_size(size_t size)
{
	uint64_t ret;

	if (size > 64)
	{
		ft_fdprintf(2, "Can't generate random larger than 64 bits\n");
		exit(1);
	}
	get_rand(&ret, sizeof(ret));
	ret = ret >> (64 - size);
	return (ret);
}
