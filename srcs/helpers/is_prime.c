/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_prime.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 02:43:25 by eduwer            #+#    #+#             */
/*   Updated: 2021/09/23 21:32:30 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

static bool	miller_rabin(uint64_t nb, uint64_t d, t_rand_gen *r)
{
	uint64_t	rand;
	uint64_t	x;
	uint64_t	i;

	rand = (get_64b_rand(r) % (nb - 4)) + 2;
	x = pow_mod(rand, d, nb);
	if (x == 1 || x == nb - 1)
		return (true);
	i = 0;
	while (d != nb - 1)
	{
		x = pow_mod(x, x, nb);
		d = d << 1;
		if (x == 1)
			return (false);
		if (x == nb - 1)
			return (true);
		i++;
	}
	return (false);
}

bool		is_prime(const uint64_t nb, uint64_t iter, t_rand_gen *r, \
				bool print_progress)
{
	uint64_t	d;

	if (nb <= 1 || nb == 4)
		return (false);
	if (nb <= 3)
		return (true);
	d = nb - 1;
	while (d & 0)
		d = d >> 1;
	while (iter > 0)
	{
		if (miller_rabin(nb, d, r) == 0)
			return (false);
		iter--;
		if (print_progress)
			ft_fdprintf(2, "+");
	}
	return (true);
}
