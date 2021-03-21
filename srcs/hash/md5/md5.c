/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/31 15:21:45 by eduwer            #+#    #+#             */
/*   Updated: 2020/03/02 22:09:29 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_hash.h>

static int	padding(t_md5_ctx *ctx)
{
	int				padding_size;
	uint64_t		nb_bits;
	unsigned char	*ret;

	padding_size = 56 - ctx->original_size % 64;
	if (padding_size <= 0)
		padding_size += 64;
	padding_size += 8;
	ret = (unsigned char *)ft_memalloc(sizeof(unsigned char) * \
		(ctx->original_size + padding_size));
	if (!ret)
		return (1);
	ft_memcpy(ret, ctx->message, ctx->original_size);
	ret[ctx->original_size] = 1 << 7 & 0xFF;
	ctx->current_size = ctx->original_size + padding_size;
	nb_bits = ctx->original_size * 8;
	ft_memcpy(&ret[ctx->current_size - 8], &nb_bits, 8);
	free(ctx->message);
	ctx->message = ret;
	return (0);
}

static void	update_pointers(t_md5_ctx *ctx)
{
	uint32_t	*mem;

	mem = ctx->buffers[3];
	ctx->buffers[3] = ctx->buffers[2];
	ctx->buffers[2] = ctx->buffers[1];
	ctx->buffers[1] = ctx->buffers[0];
	ctx->buffers[0] = mem;
}

static void	md5_loop(t_md5_ctx *ctx, int i)
{
	uint32_t	buff[16];
	int			j;

	j = -1;
	while (++j < 16)
		ft_memcpy(&buff[j], &ctx->message[i * (16 * 4) + j * 4], 4);
	ctx->savea = ctx->buffera;
	ctx->saveb = ctx->bufferb;
	ctx->savec = ctx->bufferc;
	ctx->saved = ctx->bufferd;
	j = -1;
	while (++j < 64)
	{
		universal_md5_round(ctx, j, buff);
		update_pointers(ctx);
	}
	ctx->buffera += ctx->savea;
	ctx->bufferb += ctx->saveb;
	ctx->bufferc += ctx->savec;
	ctx->bufferd += ctx->saved;
}

char		*md5_print(uint32_t bufa, uint32_t bufb, uint32_t bufc,\
				uint32_t bufd)
{
	unsigned char	conv[16];
	char			*res;
	int				i;

	if ((res = (char *)ft_memalloc(32 * sizeof(char) + 1)) == NULL)
		return (NULL);
	ft_memcpy(conv, &bufa, 4);
	ft_memcpy(conv + 4, &bufb, 4);
	ft_memcpy(conv + 8, &bufc, 4);
	ft_memcpy(conv + 12, &bufd, 4);
	i = 0;
	while (i < 16)
	{
		ft_snprintf(&res[i * 2], 2, "%02x", conv[i]);
		++i;
	}
	return (res);
}

char		*calc_md5(char *str, size_t size)
{
	size_t		i;
	t_md5_ctx	ctx;

	if ((ctx.message = (unsigned char *)ft_memalloc(size)) == NULL)
		return (NULL);
	ctx.message = ft_memcpy(ctx.message, str, size);
	ctx.original_size = size;
	if (padding(&ctx) != 0)
		return (NULL);
	ctx.buffera = 0x67452301;
	ctx.bufferb = 0xefcdab89;
	ctx.bufferc = 0x98badcfe;
	ctx.bufferd = 0x10325476;
	ctx.buffers[0] = &ctx.buffera;
	ctx.buffers[1] = &ctx.bufferb;
	ctx.buffers[2] = &ctx.bufferc;
	ctx.buffers[3] = &ctx.bufferd;
	i = 0;
	while (i < ctx.current_size / (16 * 4))
		md5_loop(&ctx, i++);
	free(ctx.message);
	return (md5_print(ctx.buffera, ctx.bufferb, ctx.bufferc, ctx.bufferd));
}
