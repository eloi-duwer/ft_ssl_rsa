/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_rsa.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 23:40:18 by eduwer            #+#    #+#             */
/*   Updated: 2021/10/02 19:04:49 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_RSA_H
# define FT_SSL_RSA_H

# include <ft_ssl.h>
# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <ft_ssl_asn1.h>

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

typedef struct 	s_genrsa_args {
	char		*filename_rand;
	char		*filename_out;
	int			fd_out;
}				t_genrsa_args;

typedef struct	s_rsa_args {
	enum e_type	in_type;
	enum e_type	out_type;
	char		*filename_in;
	int			fd_in;
	char		*filename_out;
	int			fd_out;
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

typedef struct	s_rsautl_args
{
	char		*in_file;
	char		*out_file;
	char		*inkey_file;
	bool		pubin;
	bool		decrypt;
	bool		hexdump;
}				t_rsautl_args;


static const char	*g_private_header = "-----BEGIN RSA PRIVATE KEY-----\n";
static const char	*g_private_footer = "-----END RSA PRIVATE KEY-----\n";
static const char	*g_encrypted_str = "Proc-Type: 4,ENCRYPTED\nDEK-Info: DES-CBC,";
static const char	*g_public_header = "-----BEGIN PUBLIC KEY-----\n";
static const char	*g_public_footer = "-----END PUBLIC KEY-----\n";

int					parse_genrsa_args(int ac, char **av);
int					parse_rsa_args(int ac, char **av);
int					parse_rsautl_args(int ac, char **av);
int					print_genrsa_usage(void);
int					print_rsa_usage(void);
int					print_rsautl_usage(void);
int					genrsa_process(t_genrsa_args *args);
int					rsa_process(t_rsa_args *args);
int					rsautl_process(t_rsautl_args *args);
int					get_arg(t_ac_av *a, char **pt, char *errstr);
void				check_key(t_rsa_args *args, t_rsa_key *key);

#endif
