/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asn1_dec_priv_key.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 20:12:20 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/16 16:19:45 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_asn1.h>
#include <ft_ssl_des.h>
#include <ft_ssl_rsa.h>
#include <ft_ssl_base64.h>

static int		asn1_priv_do_read_key(t_buff *buff, t_rsa_key *key)
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

static void     write_salt_des(t_des_args *args)
{
	(void)args;
}

static void		do_write_des(t_des_args *ctx, uint8_t *buff, size_t buff_size)
{
	char	*pt;

	pt = malloc(ctx->strlen_out + buff_size);
	if (pt == NULL)
		exit(print_errno("Error while writing des buffer: "));
	if (ctx->str_out)
	{
		ft_memcpy(pt, ctx->str_out, ctx->strlen_out);
		free(ctx->str_out);
	}
	ft_memcpy(&pt[ctx->strlen_out], buff, buff_size);
	ctx->str_out = pt;
	ctx->strlen_out += buff_size;
}

static void		write_des(t_des_args *ctx, uint64_t block)
{
	uint8_t	n_pad;

	block = reverse_bits_u64(block);
	if (ctx->finished_reading == true && ctx->decode == true)
	{
		n_pad = ((uint8_t *)&block)[7];
		if (n_pad <= 8)
			do_write_des(ctx, (uint8_t *)(&block), 8 - n_pad);
		else
			do_write_des(ctx, (uint8_t *)(&block), 8);
	}
	else
		do_write_des(ctx, (uint8_t *)(&block), 8);
}

static void     empty_buffer_des(t_des_args *args)
{
	(void)args;
}

static int		asn1_dec_des_priv_key(char *pts[2], t_buff *buff, char *passin)
{
	char		salt[17];
	t_des_args	des_conf;

	ft_bzero(&des_conf, sizeof(des_conf));
	if ((des_conf.password = get_pass(passin, 4, "Enter PEM pass phrase", false)) == NULL)
		return (1);
	pts[0] += ft_strlen(g_encrypted_str);
	if (pts[0] + 18 >= pts[1])
		return (print_error("The input key has a wrong format (too short)"));
	ft_strncpy(salt, pts[0], 16);
	salt[16] = '\0';
	pts[0] += 18;
	des_conf.salt_str = salt;
	des_conf.has_salt = true;
	des_conf.no_salt = true;
	des_conf.alg = cbc;
	des_conf.password_malloced = true;
	des_conf.decode = true;
	des_conf.use_default_keygen = true;
	des_conf.base64 = true;
	des_conf.str_in = pts[0];
	des_conf.strlen_in = pts[1] - pts[0];
	des_conf.write_salt_func = write_salt_des;
	des_conf.write_func = write_des;
	des_conf.empty_buffer_func = empty_buffer_des;
	if (des_process(&des_conf) == 1)
		return (1);
	buff->buff = (uint8_t *)des_conf.str_out;
	buff->buff_len = des_conf.strlen_out;
	buff->curr_len = 0;
	return (0);
}

int		asn1_dec_priv_key(char *str, size_t str_len, t_asn1_conf *conf, t_rsa_key *key)
{
	t_buff	buff;
	char		*pts[2];
	int			ret;

	if (conf->type == PEM)
	{
		pts[0] = ft_strnstr(str, g_private_header, str_len);
		if (pts[0] == NULL)
			return (print_error("ft_ssl: RSA PEM Private Header not found"));
		pts[0] += ft_strlen(g_private_header);
		pts[1] = ft_strnstr(pts[0], g_private_footer, str_len);
		if (pts[1] == NULL)
			return (print_error("ft_ssl: RSA PEM Private Footer not found"));
		if (ft_strncmp(pts[0], g_encrypted_str, ft_strlen(g_encrypted_str)) == 0)
		{
			if (asn1_dec_des_priv_key(pts, &buff, conf->pass) != 0)
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