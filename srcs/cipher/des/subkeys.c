/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subkeys.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 03:29:52 by eduwer            #+#    #+#             */
/*   Updated: 2021/01/17 13:53:59 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <ft_ssl_des.h>

static const uint8_t	g_pc_1[56] = {
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4
};

static const uint8_t	g_pc_2[48] = {
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

static uint32_t	lrotate_subkey(uint32_t key, int round)
{
	uint32_t				tmp;
	static const uint8_t	des_subkey_lshift[16] = {
		1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
	};

	tmp = 0;
	tmp = key >> (28 - des_subkey_lshift[round]);
	return ((tmp | (key << des_subkey_lshift[round])) & 0xFFFFFFF);
}

static uint64_t	join_subkeys(uint32_t one, uint32_t two)
{
	uint64_t ret;

	ret = (uint64_t)one << (28 + 8);
	ret |= (uint64_t)two << 8;
	return (ret);
}

void			gen_subkeys(t_des_args *ctx)
{
	uint64_t	subkey_pc1;
	uint32_t	halves[2];
	int			i;

	subkey_pc1 = swap_bits_u64(ctx->key, g_pc_1, 56);
	halves[0] = subkey_pc1 >> (64 - 28);
	halves[1] = (subkey_pc1 << 28) >> (64 - 28);
	i = -1;
	while (++i < 16)
	{
		halves[0] = lrotate_subkey(halves[0], i);
		halves[1] = lrotate_subkey(halves[1], i);
		ctx->subkeys[ctx->decode ? 15 - i : i] = \
			swap_bits_u64(join_subkeys(halves[0], halves[1]), g_pc_2, 48);
	}
}
