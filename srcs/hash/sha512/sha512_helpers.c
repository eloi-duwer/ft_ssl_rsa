/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha512_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 18:53:52 by eduwer            #+#    #+#             */
/*   Updated: 2020/03/03 20:45:25 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_hash.h>

/*
** Functions names are from the specification of SHA algorithms,
** cf sha256_helpers.c
*/

static uint64_t	right_rotate_64(uint64_t word, unsigned int amount)
{
	return (word >> amount | word << (64 - amount));
}

uint64_t		sha512_init(char i, uint64_t x)
{
	if (i == 0)
		return (right_rotate_64(x, 1) ^ right_rotate_64(x, 8) ^ x >> 7);
	else
		return (right_rotate_64(x, 19) ^ right_rotate_64(x, 61) ^ x >> 6);
}

uint64_t		sha512_s(char i, uint64_t x)
{
	if (i == 0)
		return (right_rotate_64(x, 28) ^ right_rotate_64(x, 34) ^ \
			right_rotate_64(x, 39));
	else
		return (right_rotate_64(x, 14) ^ right_rotate_64(x, 18) ^ \
			right_rotate_64(x, 41));
}

uint64_t		sha512_ch(uint64_t x, uint64_t y, uint64_t z)
{
	return ((x & y) ^ (~x & z));
}

uint64_t		sha512_maj(uint64_t x, uint64_t y, uint64_t z)
{
	return ((x & y) ^ (x & z) ^ (y & z));
}
