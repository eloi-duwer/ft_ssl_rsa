/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hmac_sha256.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 18:50:39 by eduwer            #+#    #+#             */
/*   Updated: 2021/01/08 04:38:26 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_hash.h>
#include <ft_ssl_des.h>

static uint8_t	*hmac_get_key(uint8_t *password, size_t size)
{
	uint8_t	*ret;

	if ((ret = (uint8_t *)ft_memalloc(64)) == NULL)
		return (NULL);
	if (size > 64)
	{
		if ((password = raw_sha256(password, size)) == NULL)
			return (NULL);
		size = 32;
	}
	ft_memcpy(ret, password, size);
	ft_memset(&ret[size], 0, 64 - size);
	return (ret);
}

uint8_t			*hmac_sha256(char *password, uint8_t *msg, size_t msg_len)
{
	uint8_t	*key;
	uint8_t	opad[64];
	uint8_t	ipad[64];
	int		i;
	uint8_t	*buff;

	if ((key = hmac_get_key((uint8_t *)password, ft_strlen(password))) == NULL)
		return (NULL);
	i = 0;
	while (i < 64)
	{
		opad[i] = key[i] ^ 0x5C;
		ipad[i] = key[i] ^ 0x36;
		i++;
	}
	free(key);
	buff = ft_memcat(ipad, 64, msg, msg_len);
	key = raw_sha256(buff, 64 + msg_len);
	free(buff);
	buff = ft_memcat(opad, 64, key, 32);
	free(key);
	key = raw_sha256(buff, 64 + 32);
	free(buff);
	return (key);
}
