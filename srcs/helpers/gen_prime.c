/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_prime.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 23:20:11 by eduwer            #+#    #+#             */
/*   Updated: 2021/04/18 19:33:59 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

uint64_t	gen_prime(ssize_t size, t_rand_gen *r)
{
	uint64_t	ret;

	ret = 0;
	while (!is_prime(ret, 42, r))
		ret = get_rand_size(r, size);
	return (ret);
}
