/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exposed_sha256.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 20:13:17 by eduwer            #+#    #+#             */
/*   Updated: 2021/01/02 01:13:27 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_hash.h>

char		*calc_sha256(char *str, size_t size)
{
	t_sha256_ctx	ctx;
	char			*ret;

	if (sha256(&ctx, str, size) == false)
		return (NULL);
	ft_asprintf(&ret, "%08x%08x%08x%08x%08x%08x%08x%08x", \
		ctx.hash[0], ctx.hash[1], ctx.hash[2], ctx.hash[3], \
		ctx.hash[4], ctx.hash[5], ctx.hash[6], ctx.hash[7]);
	return (ret);
}

uint8_t		*raw_sha256(uint8_t *str, size_t size)
{
	t_sha256_ctx	ctx;
	uint8_t			*ret;
	int				i;

	if ((ret = (uint8_t *)malloc(sizeof(uint8_t) * 32)) == NULL)
		return (NULL);
	if (sha256(&ctx, (char *)str, size) == false)
		return (NULL);
	i = 0;
	while (i < 8)
	{
		ret[4 * i] = ((uint8_t *)(&(ctx.hash[i])))[3];
		ret[4 * i + 1] = ((uint8_t *)(&(ctx.hash[i])))[2];
		ret[4 * i + 2] = ((uint8_t *)(&(ctx.hash[i])))[1];
		ret[4 * i + 3] = ((uint8_t *)(&(ctx.hash[i])))[0];
		i++;
	}
	return (ret);
}
