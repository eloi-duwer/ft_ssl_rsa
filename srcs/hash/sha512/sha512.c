/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha512.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 19:09:33 by eduwer            #+#    #+#             */
/*   Updated: 2020/03/04 15:22:48 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_hash.h>

uint64_t g_sha512_k[80] = {
	0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, \
	0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019, \
	0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, \
	0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, \
	0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, \
	0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, \
	0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275, \
	0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, \
	0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, \
	0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725, \
	0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, \
	0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df, \
	0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, \
	0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001, \
	0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218, \
	0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, \
	0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, \
	0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, \
	0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, \
	0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec, \
	0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, \
	0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207, \
	0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, \
	0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b, \
	0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, \
	0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, \
	0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

static void	init(t_sha512_ctx *ctx)
{
	ctx->hash[0] = 0x6a09e667f3bcc908;
	ctx->hash[1] = 0xbb67ae8584caa73b;
	ctx->hash[2] = 0x3c6ef372fe94f82b;
	ctx->hash[3] = 0xa54ff53a5f1d36f1;
	ctx->hash[4] = 0x510e527fade682d1;
	ctx->hash[5] = 0x9b05688c2b3e6c1f;
	ctx->hash[6] = 0x1f83d9abfb41bd6b;
	ctx->hash[7] = 0x5be0cd19137e2179;
}

int			padding_sha512(t_sha512_ctx *ctx)
{
	int				padding_size;
	UINT128			nb_bits;
	unsigned char	*ptr;
	unsigned char	*ret;
	int				i;

	padding_size = 112 - ctx->original_size % 128;
	if (padding_size <= 0)
		padding_size += 128;
	padding_size += 16;
	if ((ret = (unsigned char *)ft_memalloc(ctx->original_size + \
		padding_size)) == NULL)
		return (1);
	ft_memcpy(ret, ctx->message, ctx->original_size);
	ret[ctx->original_size] = 1 << 7 & 0xFF;
	ctx->current_size = ctx->original_size + padding_size;
	nb_bits = ctx->original_size * 8;
	ptr = (unsigned char *)&nb_bits;
	i = -1;
	while (++i < 16)
		ft_memcpy(&ret[ctx->current_size - (16 - i)], &ptr[15 - i], 1);
	free(ctx->message);
	ctx->message = ret;
	return (0);
}

static void	sha512_operation(t_sha512_ctx *ctx, int j)
{
	uint64_t	t1;
	uint64_t	t2;

	t1 = ctx->work_var[H] + sha512_s(1, ctx->work_var[E]) + \
		sha512_ch(ctx->work_var[E], ctx->work_var[F], ctx->work_var[G]) + \
		g_sha512_k[j] + ctx->work_table[j];
	t2 = sha512_s(0, ctx->work_var[A]) + \
		sha512_maj(ctx->work_var[A], ctx->work_var[B], ctx->work_var[C]);
	ctx->work_var[H] = ctx->work_var[G];
	ctx->work_var[G] = ctx->work_var[F];
	ctx->work_var[F] = ctx->work_var[E];
	ctx->work_var[E] = ctx->work_var[D] + t1;
	ctx->work_var[D] = ctx->work_var[C];
	ctx->work_var[C] = ctx->work_var[B];
	ctx->work_var[B] = ctx->work_var[A];
	ctx->work_var[A] = t1 + t2;
}

void		sha512_loop(t_sha512_ctx *ctx, int i)
{
	int	j;

	ft_memcpy(&ctx->work_var, &ctx->hash, sizeof(uint64_t) * 8);
	j = 0;
	while (j < 80)
	{
		if (j < 16)
		{
			ft_memcpy(&ctx->work_table[j], &ctx->message[i * 128 + j * 8], 8);
			ft_reverse_string((unsigned char *)&ctx->work_table[j], 8);
		}
		else
			ctx->work_table[j] = sha512_init(1, ctx->work_table[j - 2]) + \
				ctx->work_table[j - 7] + sha512_init(0, \
					ctx->work_table[j - 15]) + ctx->work_table[j - 16];
		sha512_operation(ctx, j);
		++j;
	}
	j = 0;
	while (j < 8)
	{
		ctx->hash[j] = ctx->hash[j] + ctx->work_var[j];
		++j;
	}
}

char		*calc_sha512(char *str, size_t size)
{
	t_sha512_ctx	ctx;
	size_t			i;
	char			*ret;

	if ((ctx.message = (unsigned char *)ft_memalloc(size)) == NULL)
		return (NULL);
	ft_memcpy(ctx.message, str, size);
	ctx.original_size = size;
	if (padding_sha512(&ctx) != 0)
		return (NULL);
	init(&ctx);
	i = 0;
	while (i < ctx.current_size / (16 * 8))
		sha512_loop(&ctx, i++);
	ft_asprintf(&ret, "%016lx%016lx%016lx%016lx%016lx%016lx%016lx%016lx", \
		ctx.hash[0], ctx.hash[1], ctx.hash[2], ctx.hash[3], \
		ctx.hash[4], ctx.hash[5], ctx.hash[6], ctx.hash[7]);
	free(ctx.message);
	return (ret);
}
