/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asn1_enc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 01:23:14 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/16 16:19:45 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_rsa.h>
#include <ft_ssl_base64.h>
#include <ft_ssl_asn1.h>
#include <ft_ssl.h>
#include <stdarg.h>

void		write_sequence_u64(t_buff *buff, size_t n_args, ...)
{
	va_list		ap;
	t_buff	numbers;
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

void	asn1_encode_public_key(t_rsa_key *key, t_buff *buff)
{
	t_buff	rsa_pub_id;
	t_buff	public_key;

	init_buff(&rsa_pub_id);
	init_buff(&public_key);
	write_bytes(&rsa_pub_id, g_public_rsa_id, g_public_rsa_id_len);
	write_sequence_u64(&public_key, 2, key->modulus, key->publicExponent);
	wrap_into_bit_string(&public_key);
	init_buff(buff);
	write_bytes(buff, "\x30", 1);
	write_size(buff, rsa_pub_id.curr_len + public_key.curr_len);
	write_bytes(buff, rsa_pub_id.buff, rsa_pub_id.curr_len);
	write_bytes(buff, public_key.buff, public_key.curr_len);
	erase_buff(&rsa_pub_id);
	erase_buff(&public_key);
}

char			*asn1_enc_key(t_rsa_key *key, t_asn1_conf *conf, size_t *ret_len)
{
	t_buff	buff;
	char		*ret;

	if (conf->public == true)
		asn1_encode_public_key(key, &buff);
	else
		asn1_encode_private_key(conf, key, &buff);
	if (conf->type == PEM)
	{
		if ((ret = enc_base64(buff.buff, buff.curr_len, ret_len)) == NULL)
			exit(print_errno("ft_ssl: encode base64"));
		erase_buff(&buff);
	}
	else
	{
		ret = (char *)buff.buff;
		*ret_len = buff.curr_len;
	}
	return (ret);
}

