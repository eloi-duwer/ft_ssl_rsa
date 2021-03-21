/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bufferization.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/17 16:32:52 by eduwer            #+#    #+#             */
/*   Updated: 2021/01/17 16:37:10 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <ft_ssl_base64.h>
#include <ft_ssl_des.h>

/*
** The input file is bufferized, because reading 8 bytes by 8 bytes
** is wayyyy to slow (too much syscall)
** And we have to in case of decoding from base64:
** We need to get the 16 bytes of the file for the header (Salted__)
** But encoded in base64 it is not a whole number... So we kill one bird
** with 2 stones, fixing a perf issue and allowing base64 reading
*/

static void	r_fill_check_base64(t_des_args *ctx)
{
	char	*dec;
	size_t	size;

	if (ctx->decode != true || ctx->base64 != true)
		return ;
	if ((dec = dec_base64((char *)ctx->r_buff, \
		ctx->r_buff_size, &size)) == NULL)
		exit(print_errno("Error while decoding base64: "));
	ft_memcpy(ctx->r_buff, dec, size);
	ctx->r_buff_size = size;
	free(dec);
}

/*
** Why do we try multiple reads?
** In non-canonical mode, a read on tty
** (if we read from stdin without a pipe afaik) will stop when an endl
** is reached, which means that we can have a ret from read < 8 but bytes are
** still disponible after (read can do this if you read from a socket too)
** So we loop until we have read enough bytes
*/

static void	fill_r_buffer(t_des_args *ctx)
{
	ssize_t	ret;

	ft_memset(ctx->r_buff, 0, DES_BUFF_SIZE);
	ctx->r_buff_size = 0;
	ctx->r_buff_used = 0;
	while (ctx->r_buff_size < DES_BUFF_SIZE)
	{
		ret = read(ctx->fd_in, &ctx->r_buff[ctx->r_buff_size], \
			DES_BUFF_SIZE - ctx->r_buff_size);
		if (ret == -1)
			exit(print_errno("Error while reading the file: "));
		if (ret == 0)
			return (r_fill_check_base64(ctx));
		ctx->r_buff_size += ret;
	}
	return (r_fill_check_base64(ctx));
}

ssize_t		des_get_64_bits(t_des_args *ctx, uint64_t *bl)
{
	size_t	available;

	if (ctx->fd_in == -1 || ctx->finished_reading == true)
		return (0);
	*bl = 0;
	if (ctx->r_buff_size == ctx->r_buff_used)
		fill_r_buffer(ctx);
	available = ctx->r_buff_size - ctx->r_buff_used;
	if (available < 8)
	{
		ft_memcpy(bl, &ctx->r_buff[ctx->r_buff_used], \
			available);
		ctx->r_buff_used = ctx->r_buff_size;
		return (available);
	}
	ft_memcpy(bl, &ctx->r_buff[ctx->r_buff_used], 8);
	ctx->r_buff_used += 8;
	return (8);
}
