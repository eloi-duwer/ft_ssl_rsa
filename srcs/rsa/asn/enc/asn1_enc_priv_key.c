/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asn1_enc_priv_key.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 22:57:39 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/20 22:26:01 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_rsa.h>
#include <ft_ssl_asn1.h>
#include <ft_ssl_des.h>
#include <ft_ssl_hash.h>

static void write_des(t_des_args *ctx, uint64_t block)
{
	char *pt;

	block = reverse_bits_u64(block);
	pt = malloc(ctx->strlen_out + 8);
	if (pt == NULL)
		exit(print_errno("Error while writing des buffer: "));
	if (ctx->str_out)
	{
		ft_memcpy(pt, ctx->str_out, ctx->strlen_out);
		free(ctx->str_out);
	}
	ft_memcpy(&pt[ctx->strlen_out], (uint8_t *)(&block), 8);
	ctx->str_out = pt;
	ctx->strlen_out += 8;
}

static void write_salt_des(t_des_args *ctx)
{
	(void)ctx;
}

static void empty_buffer_des(t_des_args *ctx)
{
	(void)ctx;
}

static int	do_des_conf(t_des_args *des_conf, char *pass, t_buff *buff)
{
	uint8_t	*key;

	des_conf->salt = get_64b_rand(NULL);
	des_conf->has_key = true;
	des_conf->has_salt = true;
	des_conf->password = get_pass(pass, 4, "Enter PEM pass phrase", true);
	des_conf->str_in = (char *)buff->buff;
	des_conf->strlen_in = buff->curr_len;
	des_conf->alg = cbc;
	des_conf->decode = false;
	des_conf->base64 = false;
	des_conf->write_func = write_des;
	des_conf->write_salt_func = write_salt_des;
	des_conf->empty_buffer_func = empty_buffer_des;
	if ((key = openssl_kdf(raw_md5, md5_get_ret_len(), des_conf->password, (uint8_t *)&des_conf->salt, 8, 8)) == NULL)
		return (1);
	free(des_conf->password);
	des_conf->password = NULL;
	des_conf->key = reverse_bits_u64(*(uint64_t *)key);
	free(key);
	des_conf->iv =  reverse_bits_u64(des_conf->salt);
	des_conf->has_iv = true;
	return (0);
}

void	    asn1_encode_private_key(t_asn1_conf *conf, t_rsa_key *key, t_buff *buff)
{
	t_des_args	des_conf;

	init_buff(buff);
	write_sequence_u64(buff, 9, (uint64_t)0, key->modulus, \
		key->publicExponent, key->privateExponent, key->prime1, key->prime2, \
		key->exponent1, key->exponent2, key->coefficient);
	if (conf->des == true && conf->type == PEM)
	{
		ft_bzero(&des_conf, sizeof(des_conf));
		do_des_conf(&des_conf, conf->pass, buff);
		if (des_process(&des_conf) == 1)
			exit(print_error("Error during des encryption\n"));
		erase_buff(buff);
		buff->buff = (uint8_t *)des_conf.str_out;
		buff->curr_len = des_conf.strlen_out;
		buff->buff_len = buff->curr_len;
		conf->out_salt = des_conf.salt;
	}
}