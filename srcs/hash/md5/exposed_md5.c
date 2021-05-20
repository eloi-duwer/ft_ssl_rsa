/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exposed_md5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 15:31:26 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/20 21:24:59 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_hash.h>
#include <ft_ssl.h>

char		*calc_md5(char *str, size_t size)
{
	t_md5_ctx	ctx;

	if (md5(&ctx, str, size) == false)
		return (NULL);
	return (md5_print(ctx.buffera, ctx.bufferb, ctx.bufferc, ctx.bufferd));
}

uint8_t		*raw_md5(uint8_t *str, size_t size)
{
	t_md5_ctx	ctx;
	uint8_t		*ret;

	if ((ret = (uint8_t *)malloc(sizeof(uint8_t) * 16)) == NULL)
		return (NULL);
	if (md5(&ctx, (char *)str, size) == false)
		return (NULL);
	ft_memcpy(ret, &ctx.buffera, 4);
	ft_memcpy(&ret[4], &ctx.bufferb, 4);
	ft_memcpy(&ret[8], &ctx.bufferc, 4);
	ft_memcpy(&ret[12], &ctx.bufferd, 4);
	return (ret);
}

size_t		md5_get_ret_len(void)
{
	return (16);
}