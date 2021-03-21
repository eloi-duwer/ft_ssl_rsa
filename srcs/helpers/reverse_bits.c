/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_bits.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 21:39:02 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/19 23:29:31 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

uint64_t	reverse_bits_u64(uint64_t in)
{
	int			i;
	uint64_t	ret;

	i = 0;
	ret = 0;
	while (i < 64 / 8)
	{
		ret = ret << 8;
		ret |= (in >> (i * 8)) & 0xFF;
		i++;
	}
	return (ret);
}

uint32_t	reverse_bits_u32(uint32_t in)
{
	size_t		i;
	uint32_t	ret;

	i = 0;
	ret = 0;
	while (i < 32 / 8)
	{
		ret = ret << 8;
		ret |= (in >> (i * 8)) & 0xFF;
		i++;
	}
	return (ret);
}

static const uint64_t g_one = 0x1;

/*
** Swaps the bits of in, according to the swap array of swap_len length
** if swap_len is < 64 the (64-swap_len) lower bits of the result are set to 0
** ( = right bits on high-endian, left one on little-endian)
*/

uint64_t	swap_bits_u64(uint64_t in, const uint8_t *swap, size_t swap_len)
{
	uint64_t	mask;
	uint64_t	ret;
	size_t		i;

	ret = 0;
	i = 0;
	while (i < swap_len)
	{
		mask = g_one << (64 - (swap[i]));
		if (in & mask)
			ret |= g_one << (63 - i);
		i++;
	}
	return (ret);
}

uint32_t	swap_bits_u32(uint32_t in, const uint8_t *swap, size_t swap_len)
{
	uint32_t	mask;
	uint32_t	ret;
	size_t		i;

	ret = 0;
	i = 0;
	while (i < swap_len)
	{
		mask = g_one << (32 - (swap[i]));
		if (in & mask)
			ret |= g_one << (31 - i);
		i++;
	}
	return (ret);
}
