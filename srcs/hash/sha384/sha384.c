/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha384.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 15:12:22 by eduwer            #+#    #+#             */
/*   Updated: 2020/03/04 15:20:59 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_hash.h>

static void	init(t_sha512_ctx *ctx)
{
	ctx->hash[0] = 0xcbbb9d5dc1059ed8;
	ctx->hash[1] = 0x629a292a367cd507;
	ctx->hash[2] = 0x9159015a3070dd17;
	ctx->hash[3] = 0x152fecd8f70e5939;
	ctx->hash[4] = 0x67332667ffc00b31;
	ctx->hash[5] = 0x8eb44a8768581511;
	ctx->hash[6] = 0xdb0c2e0d64f98fa7;
	ctx->hash[7] = 0x47b5481dbefa4fa4;
}

char		*calc_sha384(char *str, size_t size)
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
	ft_asprintf(&ret, "%016lx%016lx%016lx%016lx%016lx%016lx", \
		ctx.hash[0], ctx.hash[1], ctx.hash[2], \
		ctx.hash[3], ctx.hash[4], ctx.hash[5]);
	free(ctx.message);
	return (ret);
}
