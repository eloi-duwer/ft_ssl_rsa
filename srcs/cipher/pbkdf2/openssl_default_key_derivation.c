/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   openssl_default_key_derivation.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 21:29:11 by eduwer            #+#    #+#             */
/*   Updated: 2021/04/23 17:07:31 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_des.h>
#include <ft_ssl.h>
#include <ft_ssl_hash.h>

#ifdef __APPLE__
# define HASH raw_md5
# define HASH_LEN 16
#else
# define HASH raw_sha256
# define HASH_LEN 32
#endif
#define CONCAT 0
#define RET 1
#define HASH_RET 2
#define TMP 3

uint8_t		*openssl_kdf(char *password, uint8_t *salt, \
			size_t salt_len, size_t dk_len_bytes)
{
	size_t	pwd_len;
	uint8_t	*buffs[4];
	size_t	ret_len;

	pwd_len = ft_strlen(password);
	if ((buffs[CONCAT] = (uint8_t *)malloc(HASH_LEN + pwd_len + salt_len)) == NULL)
		return (NULL);
	ret_len = 0;
	buffs[RET] = NULL;
	while (ret_len < dk_len_bytes)
	{
		if (ret_len == 0)
		{
			ft_memcpy(buffs[CONCAT], password, pwd_len);
			ft_memcpy(&((buffs[CONCAT])[pwd_len]), salt, salt_len);
			buffs[HASH_RET] = HASH(buffs[CONCAT], pwd_len + salt_len);
		}
		else
		{
			ft_memcpy(buffs[CONCAT], &(buffs[RET][ret_len]), HASH_LEN);
			buffs[HASH_RET] = HASH(buffs[CONCAT], HASH_LEN + pwd_len + salt_len);
		}
		if (buffs[HASH_RET] == NULL)
			return (NULL);
		if ((buffs[TMP] = ft_memcat(buffs[RET], ret_len, buffs[HASH_RET], HASH_LEN)) == NULL)
			return (NULL);
		free(buffs[HASH_RET]);
		free(buffs[RET]);
		buffs[RET] = buffs[TMP];
		ret_len += HASH_LEN;
	}
	free(buffs[CONCAT]);
	return (buffs[RET]);
}
