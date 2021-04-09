/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genrsa.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 23:19:56 by eduwer            #+#    #+#             */
/*   Updated: 2021/04/09 14:51:12 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <ft_ssl_rsa.h>

void	key_gen(void)
{
	t_rsa_key	key;
	uint64_t	v;
	char		*b64;
	size_t		b64_len;

	ft_fdprintf(2, "Generating RSA private key, 64 bit long modulus (2 primes)\n");
	key.publicExponent = 65537;
	ft_fdprintf(2, "e is %lu (0x%x)\n", key.publicExponent, key.publicExponent);
	key.prime1 = gen_prime(32);
	key.prime2 = gen_prime(32);
	key.modulus = key.prime1 * key.prime2;
	key.coefficient = lcm(key.prime1 - 1, key.prime2 - 1);
	gcd_ext(key.publicExponent, key.coefficient, &key.privateExponent, &v);
	key.privateExponent = (key.privateExponent + key.coefficient) % key.coefficient;
	key.exponent1 = key.privateExponent % (key.prime1 - 1);
	key.exponent2 = key.privateExponent % (key.prime2 - 1);
	b64 = asn1_enc_b64_key(&key, &b64_len);
	ft_printf("-----BEGIN RSA PRIVATE KEY-----\n");
	ft_printf("%.*s\n", b64_len, b64);
	ft_printf("-----END RSA PRIVATE KEY-----\n");
	free(b64);
}
