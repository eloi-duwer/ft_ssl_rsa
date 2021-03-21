/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_rand.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 02:53:12 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/21 03:46:45 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

uint64_t	get_rand(void)
{
	static int	fd = -1;
	uint64_t	ret;

	if (fd == -1 && (fd = open("/dev/urandom", O_RDONLY)) == -1)
		exit(print_errno("ft_ssl: open /dev/urandom"));
	if (read(fd, &ret, sizeof(ret)) == -1)
		exit(print_errno("ft_ssl: read /dev/urandom"));
	return (ret);
}
