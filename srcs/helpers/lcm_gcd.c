/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcm_gcd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 23:33:02 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/27 03:52:11 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

uint64_t	gcd_ext(uint64_t a, uint64_t b, uint64_t *u, uint64_t *v)
{
	uint64_t	x;
	uint64_t	y;
	uint64_t	ret;

	if (a == 0)
	{
		*u = 0;
		*v = 1;
		return (b);
	}
	ret = gcd_ext(b % a, a, &x, &y);
	*u = y - (b / a) * x;
	*v = x;
	return (ret);
}

uint64_t	gcd(uint64_t a, uint64_t b)
{
	uint64_t	t;

	while (b != 0)
	{
		t = b;
		b = a % b;
		a = t;
	}
	return (a);
}

uint64_t	lcm(uint64_t a, uint64_t b)
{
	return ((a / gcd(a, b)) * b);
}
