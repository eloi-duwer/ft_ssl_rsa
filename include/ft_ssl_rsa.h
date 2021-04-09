/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_rsa.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 23:40:18 by eduwer            #+#    #+#             */
/*   Updated: 2021/04/09 14:50:31 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_RSA_H
# define FT_SSL_RSA_H

#include <stdint.h>
#include <stddef.h>

//Names are taken from https://tools.ietf.org/html/rfc3447#appendix-A.1

typedef struct	s_rsa_key {
	uint64_t	modulus;
	uint64_t	publicExponent;
	uint64_t	privateExponent;
	uint64_t	prime1;
	uint64_t	prime2;
	uint64_t	exponent1;
	uint64_t	exponent2;
	uint64_t	coefficient;
}				t_rsa_key;

typedef struct	s_buff {
	uint8_t		*buff;
	size_t		curr_len;
	size_t		buff_len;
}				t_buff;

char			*asn1_enc_b64_key(t_rsa_key *key, size_t *ret_len);

#endif
