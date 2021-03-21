/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 18:07:05 by eduwer            #+#    #+#             */
/*   Updated: 2020/03/03 19:05:40 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_hash.h>
#include <ft_ssl.h>

/*
** Functions names are from the specification of SHA algorithms
** cf https://csrc.nist.gov/csrc/media/publications/fips/180/2/archive/
** 2002-08-01/documents/fips180-2.pdf
** sha256_init is for functions 4.6 and 4.7 in the document,
** sha256_s is for functions 4.4 and 4.5,
** ch and maj have the same name
*/

uint32_t	sha256_init(char i, uint32_t word)
{
	if (i == 0)
		return (right_rotate(word, 7) ^ right_rotate(word, 18) ^ word >> 3);
	else
		return (right_rotate(word, 17) ^ right_rotate(word, 19) ^ word >> 10);
}

uint32_t	sha256_s(char i, uint32_t word)
{
	if (i == 1)
		return (right_rotate(word, 6) ^ right_rotate(word, 11) ^ \
			right_rotate(word, 25));
	else
		return (right_rotate(word, 2) ^ right_rotate(word, 13) ^ \
			right_rotate(word, 22));
}

uint32_t	sha256_ch(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x & y) ^ (~x & z));
}

uint32_t	sha256_maj(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x & y) ^ (x & z) ^ (y & z));
}
