/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pow_mod.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 00:07:21 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/21 03:33:37 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

static uint64_t	add_mod(uint64_t a, uint64_t b, uint64_t mod)
{
	a = a % mod;
	b = b % mod;
	if (a >= mod - b)
		return (a - (mod - b));
	return (a + b);
}

static uint64_t	mult_mod(uint64_t a, uint64_t b, uint64_t mod)
{
	uint64_t res;
	uint64_t test;

	res = 0;
	while (b > 0)
	{
		if (b & 1)
			res = add_mod(res, a, mod);
		a = add_mod(a, a, mod);
		b = b >> 1;
	}
	return (res);
}

uint64_t		pow_mod(uint64_t a, uint64_t b, uint64_t mod)
{
	uint64_t res;

	res = 1;
	a = a % mod;
	if (a == 0)
		return (0);
	while (b > 0)
	{
		if (b & 1)
			res = mult_mod(res, a, mod);
		b = b >> 1;
		a = mult_mod(a, a, mod);
	}
	return (res);
}
