/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exposed_md5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 15:31:26 by eduwer            #+#    #+#             */
/*   Updated: 2021/04/23 16:53:11 by eduwer           ###   ########.fr       */
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
	uint32_t	tmp;

	if ((ret = (uint8_t *)malloc(sizeof(uint8_t) * 16)) == NULL)
		return (NULL);
	if (md5(&ctx, (char *)str, size) == false)
		return (NULL);
	tmp = reverse_bits_u32(ctx.buffera);
	ft_memcpy(ret, &tmp, 4);
	tmp = reverse_bits_u32(ctx.bufferb);
	ft_memcpy(&ret[4], &tmp, 4);
	tmp = reverse_bits_u32(ctx.bufferc);
	ft_memcpy(&ret[8], &tmp, 4);
	tmp = reverse_bits_u32(ctx.bufferd);
	ft_memcpy(&ret[12], &tmp, 4);
	return (ret);
}
