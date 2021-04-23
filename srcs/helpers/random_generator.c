/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_generator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 19:16:26 by eduwer            #+#    #+#             */
/*   Updated: 2021/04/18 19:40:13 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

void	init_rand(t_rand_gen *r, char *filename)
{
	if (filename == NULL)
		r->fd = open("/dev/urandom", O_RDONLY);
	else
		r->fd = open(filename, O_RDONLY);
	if (r->fd == -1)
		exit(print_errno("ft_ssl: open rand file"));
}

void	destroy_rand(t_rand_gen *r)
{
	close(r->fd);
}

void	get_rand(t_rand_gen *r, void *buff, ssize_t size)
{
	ssize_t		ret_size;
	t_rand_gen	tmp;
	int			fd;

	if (r == NULL)
	{
		init_rand(&tmp, NULL);
		fd = tmp.fd;
	}
	else
		fd = r->fd;
	if (buff != NULL)
	{
		ret_size = read(fd, buff, size);
		if (ret_size == -1)
			exit(print_errno("ft_ssl: read rand file"));
		else if (ret_size != (ssize_t)size)
			exit(print_error("ft_ssl: Can't get more rand data from rand file"));
	}
	if (r == NULL)
		destroy_rand(&tmp);
}

uint64_t	get_64b_rand(t_rand_gen *r)
{
	uint64_t rand;

	get_rand(r, &rand, sizeof(rand));
	return (rand);
}

uint64_t	get_rand_size(t_rand_gen *r, size_t size)
{
	uint64_t ret;

	if (size > 64)
	{
		ft_fdprintf(2, "Can't generate random larger than 64 bits\n");
		exit(1);
	}
	get_rand(r, &ret, sizeof(ret));
	ret = ret >> (64 - size);
	return (ret);
}
