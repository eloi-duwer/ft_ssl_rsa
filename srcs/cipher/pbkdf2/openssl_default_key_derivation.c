/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   openssl_default_key_derivation.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 21:29:11 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/20 22:03:15 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_des.h>
#include <ft_ssl.h>
#include <ft_ssl_hash.h>

static int	concat_buff(t_buff *ret, t_buff *buff, char *password, size_t pw_len, uint8_t *salt, size_t salt_len)
{
	uint8_t	*tmp;

	tmp = (uint8_t *)malloc(buff->buff_len + pw_len + salt_len);
	if (tmp == NULL)
		return (print_errno("ft_ssl: malloc error: "));
	ft_memcpy(tmp, buff->buff, buff->buff_len);
	ft_memcpy(tmp + buff->buff_len, password, pw_len);
	ft_memcpy(tmp + buff->buff_len + pw_len, salt, salt_len);
	ret->buff = tmp;
	ret->buff_len = buff->buff_len + pw_len + salt_len;
	return (0);
}

static int	concat_ret(t_buff *ret, t_buff *block)
{
	uint8_t	*tmp;

		tmp = (uint8_t *)malloc(ret->buff_len + block->buff_len);
	if (tmp == NULL)
		return (print_errno("ft_ssl: malloc error: "));
	ft_memcpy(tmp, ret->buff, ret->buff_len);
	ft_memcpy(tmp + ret->buff_len, block->buff, block->buff_len);
	free(ret->buff);
	ret->buff = tmp;
	ret->buff_len = ret->buff_len + block->buff_len;
	return (0);
}

uint8_t		*openssl_kdf(uint8_t *(*hash_func)(uint8_t *, size_t), size_t hash_len, \
	char *password, uint8_t *salt, size_t salt_len, size_t target_len)
{
	t_buff	ret;
	t_buff	block;
	t_buff	hash_ret;

	ft_bzero(&ret, sizeof(ret));
	ft_bzero(&block, sizeof(block));
	ft_bzero(&hash_ret, sizeof(hash_ret));
	while (ret.buff_len < target_len)
	{
		if (concat_buff(&block, &hash_ret, password, ft_strlen(password), salt, salt_len) == 1)
			return (NULL);
		erase_buff(&hash_ret);
		if ((hash_ret.buff = hash_func(block.buff, block.buff_len)) == NULL)
		{
			print_errno("ft_ssl: Error during hash: ");
			return (NULL);
		}
		hash_ret.buff_len = hash_len;
		erase_buff(&block);
		if (concat_ret(&ret, &hash_ret) == 1)
			return (NULL);
	}
	erase_buff(&hash_ret);
	return (ret.buff);
}
