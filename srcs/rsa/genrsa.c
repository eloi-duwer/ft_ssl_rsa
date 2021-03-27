/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genrsa.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 23:19:56 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/27 04:06:01 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <ft_ssl_rsa.h>

void	key_gen(void)
{
	t_rsa_key	key;
	uint64_t	v;

	ft_fdprintf(2, "Generating RSA private key, 64 bit long modulus (2 primes)\n");
	key.exponent = 65537;
	ft_fdprintf(2, "e is %lu (0x%x)\n", key.exponent, key.exponent);
	key.p = gen_prime(30);
	key.q = gen_prime(34);
	key.n = key.p * key.q;
	key.lambda = lcm(key.p - 1, key.q - 1);
	gcd_ext(key.exponent, key.lambda, &key.d, &v);
	key.d = (key.d + key.lambda) % key.lambda;
	//ft_printf("%lu %lu %lu %lu %lu\n", key.p, key.q, key.n, key.lambda, key.d);
	ft_printf("-----BEGIN RSA PRIVATE KEY-----\n");
	ft_printf("-----END RSA PRIVATE KEY-----\n");
}
