/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asn1_enc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 01:23:14 by eduwer            #+#    #+#             */
/*   Updated: 2021/04/09 14:47:54 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_rsa.h>
#include <ft_ssl_base64.h>
#include <ft_ssl.h>
#include <stdarg.h>

static void		init_buff(t_buff *buff)
{
	buff->curr_len = 0;
	buff->buff_len = 512;
	if ((buff->buff = (uint8_t *)malloc(buff->buff_len)) == NULL)
		exit(print_errno("ft_ssl (malloc)"));
}

static void		erase_buff(t_buff *buff)
{
	if (buff->buff != NULL)
	{
		ft_bzero(buff->buff, buff->buff_len);
		free(buff->buff);
	}
	buff->buff_len = 0;
	buff->curr_len = 0;
}

static void		write_bytes(t_buff *buff, void *bytes, size_t len)
{
	uint8_t	*tmp;
	size_t	max;

	if (len + buff->curr_len > buff->buff_len)
	{
		max = buff->buff_len * 2 > len ? buff->buff_len * 2 : len;
		if ((tmp = (uint8_t *)malloc(max)) == NULL)
			exit(print_errno("ft_ssl (malloc)"));
		ft_memcpy(tmp, buff->buff, buff->curr_len);
		buff->buff_len = max;
	}
	ft_memcpy(&buff->buff[buff->curr_len], bytes, len);
	buff->curr_len += len;
}

static void		write_size(t_buff *buff, size_t n)
{
	uint8_t	b;
	uint8_t	c;

	if (n < 128)
	{
		b = 0xFF & n;
		write_bytes(buff, &b, 1);
		return ;
	}
	b = n / 256;
	write_bytes(buff, &b, 1);
	c = 0;
	while (c <= n / 256)
	{
		b = (n >> (8 * (n / 256 - c))) & 0xFF;
		write_bytes(buff, &b, 1);
		c++;
	}
}

static void		write_integer(t_buff *buff, uint64_t n)
{
	uint8_t	n_bytes;
	uint8_t	tmp;
	bool	zero_pad;

	write_bytes(buff, "\x02", 1);
	if (n == 0)
		n_bytes = 0;
	else
	{
		n_bytes = 7;
		while (((n >> (8 * n_bytes)) & 0xFF) == 0)
			n_bytes--;
	}
	if ((n >> (8 * n_bytes)) & 0x80)
	{
		zero_pad = true;
		n_bytes++;
	}
	else
		zero_pad = false;
	n_bytes++;
	write_bytes(buff, &n_bytes, 1);
	if (zero_pad)
	{
		write_bytes(buff, "\x00", 1);
		n_bytes--;
	}
	while (n_bytes-- > 0)
	{
		tmp = (n >> (8 * n_bytes)) & 0xFF;
		write_bytes(buff, &tmp, 1);
	}
}

static void		write_sequence_u64(t_buff *buff, size_t n_args, ...)
{
	va_list		ap;
	t_buff		numbers;
	uint64_t	n;

	init_buff(&numbers);
	va_start(ap, n_args);
	while (n_args > 0)
	{
		n = va_arg(ap, uint64_t);
		write_integer(&numbers, n);
		n_args--;
	}
	write_bytes(buff, "\x30", 1);
	write_size(buff, numbers.curr_len);
	write_bytes(buff, numbers.buff, numbers.curr_len);
	erase_buff(&numbers);
}

char			*asn1_enc_b64_key(t_rsa_key *key, size_t *ret_len)
{
	t_buff	buff;
	char	*ret;

	init_buff(&buff);
	write_sequence_u64(&buff, 9, (uint64_t)0, key->modulus, \
		key->publicExponent, key->privateExponent, key->prime1, key->prime2, \
		key->exponent1, key->exponent2, key->coefficient);
	if ((ret = enc_base64(buff.buff, buff.curr_len, ret_len)) == NULL)
		exit(print_errno("ft_ssl: enc_base64"));
	erase_buff(&buff);
	return (ret);
}

