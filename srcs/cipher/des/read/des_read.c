/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 02:41:42 by eduwer            #+#    #+#             */
/*   Updated: 2021/01/17 16:33:19 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <ft_ssl_des.h>

static bool		set_finished_reading(t_des_args *ctx)
{
	close(ctx->fd_in);
	ctx->finished_reading = true;
	return (true);
}

/*
** Decoding mode is quite a hassle, because of the padding
** there is ALWAYS padding at the end, even if we don't need it
** (so that the presence of padding is not ambiguous)
** Which means that we CAN have 8 bytes of padding,
** We can't know it in advance. The solution is to add a read Buffer
** So that we always try to read what is after what we return, and we
** can know if the current block contains the padding or not
** We could do it by reading the entire file, but it's quite gross and
** ram-ineficient (yes it has been done everywhere else shhhhhhh)
** This trickery should not be needed elsewere, as there is no padding
** on the other algs
*/

bool			des_get_next_block(t_des_args *ctx, uint64_t *bl)
{
	ssize_t	ret;

	if (ctx->finished_reading == true)
		return (false);
	*bl = ctx->n_bl;
	if (ctx->next_block_is_last == true)
		return (set_finished_reading(ctx));
	ret = des_get_64_bits(ctx, &ctx->n_bl);
	ctx->n_bl = reverse_bits_u64(ctx->n_bl);
	if (ret < 8)
	{
		ctx->next_block_is_last = true;
		if (ctx->decode == false)
			ft_memset(&ctx->n_bl, 8 - ret, 8 - ret);
		else if (ret != 0)
			exit(print_error(\
				"Error: File size is expected to be a multiple of 8 bytes"));
		else
			return (set_finished_reading(ctx));
	}
	return (true);
}

void			des_init_next_block(t_des_args *ctx)
{
	ssize_t	ret;

	ret = des_get_64_bits(ctx, &ctx->n_bl);
	ctx->n_bl = reverse_bits_u64(ctx->n_bl);
	if (ret < 8)
	{
		ctx->next_block_is_last = true;
		if (ctx->decode == false)
			ft_memset(&ctx->n_bl, 8 - ret, 8 - ret);
		else if (ret != 0)
			exit(print_error(\
				"Error: File size is expected to be a multiple of 8 bytes"));
	}
}
