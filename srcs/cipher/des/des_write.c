/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_write.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 23:55:31 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/12 14:23:14 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <ft_ssl_base64.h>
#include <ft_ssl_des.h>

void		des_empty_buffer(t_des_args *ctx)
{
	uint8_t	*str;
	size_t	size;

	if (ctx->w_buff_size == 0)
		return ;
	if (ctx->decode == false && ctx->base64 == true)
	{
		if ((str = (uint8_t *)enc_base64((char *)ctx->w_buff, \
			ctx->w_buff_size, &size)) == NULL)
			exit(print_errno("Error while encoding to base64: "));
	}
	else
	{
		str = ctx->w_buff;
		size = ctx->w_buff_size;
	}
	if (write(ctx->fd_out, str, size) != (ssize_t)size)
		exit(print_errno("Error while writing: "));
	ctx->w_buff_size = 0;
	ft_memset(ctx->w_buff, 0, ctx->w_buff_size);
	if (ctx->decode == false && ctx->base64 == true)
		free(str);
}

static void	des_write_bufferized(t_des_args *ctx, uint8_t *buff, \
	size_t buff_size)
{
	while (buff_size > 0)
	{
		if (ctx->w_buff_size + buff_size <= DES_BUFF_SIZE)
		{
			ft_memcpy(&ctx->w_buff[ctx->w_buff_size], buff, buff_size);
			ctx->w_buff_size += buff_size;
			buff_size = 0;
		}
		else
		{
			ft_memcpy(&ctx->w_buff[ctx->w_buff_size], buff, \
				DES_BUFF_SIZE - ctx->w_buff_size);
			buff_size -= DES_BUFF_SIZE - ctx->w_buff_size;
			buff += DES_BUFF_SIZE - ctx->w_buff_size;
			ctx->w_buff_size = DES_BUFF_SIZE;
		}
		if (ctx->w_buff_size == DES_BUFF_SIZE)
			des_empty_buffer(ctx);
	}
}

void		des_write_to_file(t_des_args *ctx, uint64_t block)
{
	uint8_t	n_pad;

	block = reverse_bits_u64(block);
	if (ctx->decode == true && ctx->finished_reading == true)
	{
		n_pad = ((uint8_t *)&block)[7];
		if (n_pad <= 8)
			des_write_bufferized(ctx, (uint8_t *)(&block), \
				8 - n_pad);
		else
			des_write_bufferized(ctx, (uint8_t *)(&block), 8);
	}
	else
		des_write_bufferized(ctx, (uint8_t *)(&block), 8);
}

void		des_write_salt_to_file(t_des_args *ctx)
{
	uint64_t	salt;

	if (ctx->decode == true || ctx->no_salt == true)
		return ;
	salt = reverse_bits_u64(ctx->salt);
	des_write_bufferized(ctx, (uint8_t *)"Salted__", 8);
	des_write_bufferized(ctx, (uint8_t *)&salt, 8);
}
