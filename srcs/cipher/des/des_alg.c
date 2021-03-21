/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_alg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 17:13:04 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/12 12:30:26 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <ft_ssl_des.h>

/*
**	Initial Permutation for a 64-bit block
*/

static const uint8_t	g_ip[64] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

/*
**	Reverse Initial Permutation for a 64-bit block
*/

static const uint8_t	g_rip[64] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25
};

static const uint8_t	g_expand[48] = {
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1
};

static uint32_t	swap_boxes(uint32_t half, uint64_t subkey)
{
	uint64_t	expanded_half;

	expanded_half = ((uint64_t)half) << 32;
	expanded_half = swap_bits_u64(expanded_half, g_expand, 48);
	expanded_half = subkey ^ expanded_half;
	return (s_boxes(expanded_half));
}

/*
** halves: Left then Right half of the block,
** For the current round and the precendent
*/

static uint64_t	des_enc(t_des_args *ctx, uint64_t block)
{
	int			i;
	uint32_t	halves[2][2];

	block = swap_bits_u64(block, g_ip, 64);
	halves[0][0] = block >> 32;
	halves[0][1] = block & 0xFFFFFFFF;
	i = 0;
	while (i < 16)
	{
		halves[1][0] = halves[0][1];
		halves[1][1] = halves[0][0] ^ swap_boxes(halves[0][1], ctx->subkeys[i]);
		halves[0][0] = halves[1][0];
		halves[0][1] = halves[1][1];
		i++;
	}
	return (swap_bits_u64(((uint64_t)halves[0][1] << 32) | halves[0][0], \
		g_rip, 64));
}

static void		des_block_handle_cbc(t_des_args *ctx, uint64_t block)
{
	uint64_t	ret;
	uint64_t	iv;

	if (ctx->alg == cbc)
	{
		if (ctx->decode == false)
			block = block ^ ctx->iv;
		else
		{
			iv = ctx->iv;
			ctx->iv = block;
		}
	}
	ret = des_enc(ctx, block);
	if (ctx->alg == cbc)
	{
		if (ctx->decode == false)
			ctx->iv = ret;
		else
			ret = ret ^ iv;
	}
	des_write_to_file(ctx, ret);
}

int				des_loop_blocks(t_des_args *ctx)
{
	uint64_t	block;

	des_write_salt_to_file(ctx);
	while (des_get_next_block(ctx, &block))
		des_block_handle_cbc(ctx, block);
	des_empty_buffer(ctx);
	close(ctx->fd_out);
	return (0);
}
