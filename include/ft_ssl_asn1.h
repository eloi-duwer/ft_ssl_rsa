/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_asn1.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 19:06:55 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/20 19:00:36 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_ASN1_H
# define FT_SSL_ASN1_H

# include <ft_ssl.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

struct s_rsa_key;

enum e_type {
	PEM,
	DER
};

typedef struct	s_asn1_conf {
	enum e_type	type;
	bool		public;
	bool		des;
	char		*pass;
	uint64_t	out_salt;
}				t_asn1_conf;

//Header with identifier 1.2.840.113549.1.1.1, identifying a rsa public key if i understand correctly
static char 	*g_public_rsa_id 	= "\x30\x0D\x06\x09\x2A\x86\x48\x86\xF7\x0D\x01\x01\x01\x05\x00";
//Id ends with \x00, which would break strlen
static size_t	g_public_rsa_id_len = 15;

char				*asn1_enc_key(struct s_rsa_key *key, t_asn1_conf *conf, size_t *ret_len);
void				asn1_encode_private_key(t_asn1_conf *conf, struct s_rsa_key *key, t_buff *buff);
int					asn1_dec_key(char *str, size_t str_len, t_asn1_conf *conf, struct s_rsa_key *ret);
int					asn1_dec_priv_key(char *str, size_t str_len, t_asn1_conf *conf, struct s_rsa_key *key);
void				write_bytes(t_buff *buff, void *bytes, size_t len);
void				write_size(t_buff *buff, size_t n);
void				write_integer(t_buff *buff, uint64_t n);
void				wrap_into_bit_string(t_buff *buff);
void				write_sequence_u64(t_buff *buff, size_t n_args, ...);

size_t				asn1_read_object(t_buff *buff, uint8_t identifier);
uint64_t			asn1_read_integer(t_buff *buff);
size_t				asn1_read_size(t_buff *buff);
uint8_t				asn1_read_byte(t_buff *buff);

#endif
