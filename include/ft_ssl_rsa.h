/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_rsa.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 23:40:18 by eduwer            #+#    #+#             */
/*   Updated: 2021/04/18 21:29:08 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_RSA_H
# define FT_SSL_RSA_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>

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

typedef struct 	s_genrsa_args {
	char		*filename_rand;
	char		*filename_out;
	int			fd_out;
}				t_genrsa_args;

enum e_type {
	PEM,
	DER
};

typedef struct	s_rsa_args {
	enum e_type	in_type;
	enum e_type	out_type;
	char		*filename_in;
	char		*filename_out;
	char		*passin;
	char		*passout;
	bool		des;
	bool		text;
	bool		noout;
	bool		modulus;
	bool		check;
	bool		pubin;
	bool		pubout;
}				t_rsa_args;

char			*asn1_enc_b64_key(t_rsa_key *key, size_t *ret_len);
int				parse_genrsa_args(int ac, char **av);
int				print_genrsa_usage(void);
int				genrsa_process(t_genrsa_args *args);

#endif
