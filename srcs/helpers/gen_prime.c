/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_prime.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 23:20:11 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/26 23:29:50 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

uint64_t	gen_prime(size_t size)
{
	uint64_t	ret;

	ret = 0;
	while (!is_prime(ret, 42))
		ret = get_rand_size(size);
	return (ret);
}
