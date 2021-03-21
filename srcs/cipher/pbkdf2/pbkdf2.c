/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pbkdf2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 20:48:41 by eduwer            #+#    #+#             */
/*   Updated: 2021/01/09 01:16:11 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_des.h>
#include <ft_ssl.h>

/*
**	Usage of the buffs:
**	0 = msg for hmac, salt + i for first iteration,
**		previous hmac for the others
**	1 = return from hmac
**	2 = global return, xored with the return of hmac each iterations
*/

#include <stdio.h>

static uint8_t	*calc_block(char *password, uint8_t *salt, \
	size_t salt_len, uint32_t be_i)
{
	uint8_t	*buffs[3];
	size_t	iter;

	if ((buffs[0] = ft_memcat(salt, salt_len, &be_i, \
		sizeof(uint32_t))) == NULL)
		return (NULL);
	if ((buffs[2] = (uint8_t *)malloc(32)) == NULL)
		return (NULL);
	iter = 0;
	while (iter < 8192)
	{
		buffs[1] = hmac_sha256(password, buffs[0], \
			iter == 0 ? salt_len + sizeof(uint32_t) : 32);
		free(buffs[0]);
		buffs[0] = buffs[1];
		if (iter == 0)
			ft_memcpy(buffs[2], buffs[1], 32);
		else
			xor_array(buffs[2], buffs[1], 32);
		iter++;
	}
	free(buffs[1]);
	return (buffs[2]);
}

/*
**	Usage of the buffs:
**	0 : result of calc_t
**	1 : return of the concatenation
**	2 : return buffer
**	Salt is expected high endian, if provided by the user
*/

uint8_t			*pbkdf2_hmac_sha256(char *password, uint8_t *salt, \
					size_t salt_len, size_t dk_len_bytes)
{
	uint8_t	*buffs[3];
	size_t	i;

	i = 0;
	buffs[2] = NULL;
	while (i != (dk_len_bytes / 32) + (dk_len_bytes % 32 != 0))
	{
		if ((buffs[0] = calc_block(password, salt, salt_len, \
			reverse_bits_u32(i + 1))) == NULL \
			|| (buffs[1] = ft_memcat(buffs[2], i * 32, buffs[0], 32)) == NULL)
			return (NULL);
		free(buffs[2]);
		free(buffs[0]);
		buffs[2] = buffs[1];
		i++;
	}
	return (buffs[2]);
}
