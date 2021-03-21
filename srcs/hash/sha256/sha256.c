/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 15:02:15 by eduwer            #+#    #+#             */
/*   Updated: 2020/12/31 17:55:38 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_hash.h>

uint32_t g_sha256_k[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, \
	0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, \
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, \
	0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, \
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, \
	0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, \
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, \
	0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, \
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, \
	0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, \
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static void	init(t_sha256_ctx *ctx)
{
	ctx->hash[0] = 0x6a09e667;
	ctx->hash[1] = 0xbb67ae85;
	ctx->hash[2] = 0x3c6ef372;
	ctx->hash[3] = 0xa54ff53a;
	ctx->hash[4] = 0x510e527f;
	ctx->hash[5] = 0x9b05688c;
	ctx->hash[6] = 0x1f83d9ab;
	ctx->hash[7] = 0x5be0cd19;
}

static int	padding(t_sha256_ctx *ctx)
{
	int				padding_size;
	uint64_t		nb_bits;
	unsigned char	*ret;
	int				i;
	unsigned char	*ptr;

	padding_size = 56 - ctx->original_size % 64;
	if (padding_size <= 0)
		padding_size += 64;
	padding_size += 8;
	if ((ret = (unsigned char *)ft_memalloc(sizeof(unsigned char) * \
		(ctx->original_size + padding_size))) == NULL)
		return (1);
	ft_memcpy(ret, ctx->message, ctx->original_size);
	ret[ctx->original_size] = 1 << 7 & 0xFF;
	ctx->current_size = ctx->original_size + padding_size;
	nb_bits = ctx->original_size * 8;
	ptr = (unsigned char *)&nb_bits;
	i = -1;
	while (++i < 8)
		ft_memcpy(&ret[ctx->current_size - (8 - i)], &ptr[7 - i], 1);
	free(ctx->message);
	ctx->message = ret;
	return (0);
}

static void	sha256_operation(t_sha256_ctx *ctx, int j)
{
	uint32_t	t1;
	uint32_t	t2;

	t1 = ctx->work_var[H] + sha256_s(1, ctx->work_var[E]) + \
		sha256_ch(ctx->work_var[E], ctx->work_var[F], ctx->work_var[G]) + \
		g_sha256_k[j] + ctx->work_table[j];
	t2 = sha256_s(0, ctx->work_var[A]) + \
		sha256_maj(ctx->work_var[A], ctx->work_var[B], ctx->work_var[C]);
	ctx->work_var[H] = ctx->work_var[G];
	ctx->work_var[G] = ctx->work_var[F];
	ctx->work_var[F] = ctx->work_var[E];
	ctx->work_var[E] = ctx->work_var[D] + t1;
	ctx->work_var[D] = ctx->work_var[C];
	ctx->work_var[C] = ctx->work_var[B];
	ctx->work_var[B] = ctx->work_var[A];
	ctx->work_var[A] = t1 + t2;
}

static void	sha256_loop(t_sha256_ctx *ctx, int i)
{
	int			j;

	ft_memcpy(&ctx->work_var, &ctx->hash, sizeof(uint32_t) * 8);
	j = 0;
	while (j < 64)
	{
		if (j < 16)
		{
			ft_memcpy(&ctx->work_table[j], &ctx->message[i * (64) + j * 4], 4);
			ft_reverse_string((unsigned char *)&ctx->work_table[j], 4);
		}
		else
			ctx->work_table[j] = sha256_init(1, ctx->work_table[j - 2]) + \
				ctx->work_table[j - 7] + sha256_init(0, \
					ctx->work_table[j - 15]) + ctx->work_table[j - 16];
		sha256_operation(ctx, j);
		++j;
	}
	j = 0;
	while (j < 8)
	{
		ctx->hash[j] = ctx->hash[j] + ctx->work_var[j];
		++j;
	}
}

bool		sha256(t_sha256_ctx *ctx, char *str, size_t size)
{
	size_t			i;

	if ((ctx->message = (unsigned char *)ft_memalloc(size)) == NULL)
		return (false);
	ft_memcpy(ctx->message, str, size);
	ctx->original_size = size;
	if (padding(ctx) != 0)
		return (false);
	init(ctx);
	i = 0;
	while (i < ctx->current_size / 64)
		sha256_loop(ctx, i++);
	free(ctx->message);
	return (true);
}
