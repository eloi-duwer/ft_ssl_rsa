/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asn1_dec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/29 23:29:19 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/07 16:18:54 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_asn1.h>
#include <ft_ssl_rsa.h>
#include <ft_ssl_base64.h>

static int		asn1_pub_do_read_key(t_asn1_buff *buff, t_rsa_key *key)
{
	asn1_read_object(buff, '\x30');
	if (buff->curr_len + g_public_rsa_id_len > buff->buff_len \
		|| ft_memcmp(buff->buff + buff->curr_len, g_public_rsa_id, g_public_rsa_id_len) != 0)
		return (print_error("ASN.1: Cannot read this public key format (invalid Object identifier)"));
	buff->curr_len += g_public_rsa_id_len;
	asn1_read_object(buff, '\x03');
	if (asn1_read_byte(buff) != '\x00')
		return (print_error("ASN.1: Unexpected value for Bit String padding (expected \\x00)"));
	asn1_read_object(buff, '\x30');
	key->modulus = asn1_read_integer(buff);
	key->publicExponent = asn1_read_integer(buff);
	if (buff->curr_len != buff->buff_len)
		return (print_error("ASN.1: Extraneous data after the end of RSA public key"));
	return (0);
}

static int		asn1_dec_pub_key(char *str, size_t str_len, t_asn1_conf *conf, t_rsa_key *key)
{
	t_asn1_buff	buff;
	char		*pts[2];
	int			ret;

	if (conf->type == PEM)
	{
		pts[0] = ft_strnstr(str, g_public_header, str_len);
		if (pts[0] == NULL)
			return (print_error("ft_ssl: RSA PEM Public Header not found"));
		pts[0] += ft_strlen(g_public_header);
		pts[1] = ft_strnstr(pts[0], g_public_footer, str_len);
		if (pts[1] == NULL)
			return (print_error("ft_ssl: RSA PEM Public Footer not found"));
		if ((buff.buff = dec_base64(pts[0], pts[1] - pts[0], &buff.buff_len)) == NULL)
			return (print_errno("ft_ssl: error while decoding base64: "));
		buff.curr_len = 0;
	}
	else
	{
		buff.buff = (uint8_t *)str;
		buff.curr_len = 0;
		buff.buff_len = str_len;
	}
	ret = asn1_pub_do_read_key(&buff, key);
	if (conf->type == PEM)
		free(buff.buff);
	return (ret);
}

static int		asn1_priv_do_read_key(t_asn1_buff *buff, t_rsa_key *key)
{
	uint64_t	tmp;
	char		*err;

	asn1_read_object(buff, '\x30');
	if ((tmp = asn1_read_integer(buff)) != 0)
	{
		ft_asprintf(&err, "ASN.1: Unexpected version for RSA private key: expected 0, got %.02d\n", tmp);
		print_error(err);
		free(err);
		return (1);
	}
	key->modulus = asn1_read_integer(buff);
	key->publicExponent = asn1_read_integer(buff);
	key->privateExponent = asn1_read_integer(buff);
	key->prime1 = asn1_read_integer(buff);
	key->prime2 = asn1_read_integer(buff);
	key->exponent1 = asn1_read_integer(buff);
	key->exponent2 = asn1_read_integer(buff);
	key->coefficient = asn1_read_integer(buff);
	if (buff->curr_len != buff->buff_len)
		return (print_error("ASN.1: Extraneous data after the end of RSA private key"));
	return (0);
}

static int		asn1_dec_des_priv_key(char *pts[2], t_asn1_buff *buff, char *passin)
{
	char	*pass;
	uint8_t	*key_iv;

	if ((pass = get_pass(passin, 4, "Enter PEM pass phrase: ")) == NULL)
		return (1);
	pts[0] += ft_strlen(g_encrypted_str);
	
	return (0);
}

static int		asn1_dec_priv_key(char *str, size_t str_len, t_asn1_conf *conf, t_rsa_key *key, char *passin)
{
	t_asn1_buff	buff;
	char		*pts[2];
	int			ret;

	if (conf->type == PEM)
	{
		pts[0] = ft_strnstr(str, g_private_header, str_len);
		if (pts[0] == NULL)
			return (print_error("ft_ssl: RSA PEM Private Header not found"));
		pts[0] += ft_strlen(g_public_header);
		pts[1] = ft_strnstr(pts[0], g_private_footer, str_len);
		if (pts[1] == NULL)
			return (print_error("ft_ssl: RSA PEM Private Footer not found"));
		if (ft_strncmp(pts[0], g_encrypted_str, ft_strlen(g_encrypted_str)) == 0)
		{
			if (asn1_dec_des_priv_key(pts, &buff, passin) != 0)
				return (1);
		}
		else
		{
			if ((buff.buff = dec_base64(pts[0], pts[1] - pts[0], &buff.buff_len)) == NULL)
				return (print_errno("ft_ssl: Error while decoding base64: "));
			buff.curr_len = 0;
		}
	}
	else
	{
		buff.buff = (uint8_t *)str;
		buff.curr_len = 0;
		buff.buff_len = str_len;
	}
	ret = asn1_priv_do_read_key(&buff, key);
	if (conf->type == PEM)
		free(buff.buff);
	return (ret);
}

int				asn1_dec_key(char *str, size_t str_len, t_asn1_conf *conf, t_rsa_key *ret, char *passin)
{
	if (conf->public == true)
	{
		if (asn1_dec_pub_key(str, str_len, conf, ret) != 0)
			return (1);
	}
	else if (asn1_dec_priv_key(str, str_len, conf, ret, passin) != 0)
		return (1);
	return (0);
}
