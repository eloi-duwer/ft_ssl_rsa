/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 02:54:27 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/12 15:02:33 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <ft_ssl_des.h>

static int		conv_strs(t_des_args *ctx)
{
	if (ctx->key_str != NULL && (ctx->has_key = true))
		ctx->key = ft_char_to_hex_u64(ctx->key_str);
	if (ctx->salt_str != NULL && (ctx->has_salt = true))
		ctx->salt = ft_char_to_hex_u64(ctx->salt_str);
	if (ctx->iv_str != NULL && (ctx->has_iv = true))
		ctx->iv = ft_char_to_hex_u64(ctx->iv_str);
	ctx->fd_in = 0;
	if (ctx->filename_in != NULL && \
		(ctx->fd_in = open(ctx->filename_in, O_RDONLY)) == -1)
		return (print_errno("Can't open input file: "));
	ctx->fd_out = 1;
	if (ctx->filename_out != NULL && (ctx->fd_out = \
		open(ctx->filename_out, O_RDWR | O_CREAT | O_TRUNC, 0777)) == -1)
		return (print_errno("Can't open output file: "));
	return (0);
}

int				des_process(t_des_args *ctx)
{
	if (conv_strs(ctx) != 0)
		return (1);
	if (ctx->has_key == false && ctx->password == NULL)
		get_password(ctx);
	des_init_next_block(ctx);
	get_salt(ctx);
	if (ctx->alg != ecb && ctx->has_key == true && ctx->has_iv == false)
		return (print_error("All des modes (except ecb) needs an iv"));
	if (ctx->has_key == false)
		get_key_iv(ctx);
	if (ctx->print_keys)
	{
		ft_printf("Salt= %.16lX\n", ctx->salt);
		ft_printf("Key = %.16lX\n", ctx->key);
		if (ctx->alg == cbc)
			ft_printf("Iv  = %.16lX\n", ctx->iv);
	}
	gen_subkeys(ctx);
	return (des_loop_blocks(ctx));
}
