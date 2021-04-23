/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genrsa.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 23:19:56 by eduwer            #+#    #+#             */
/*   Updated: 2021/04/18 19:40:54 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <ft_ssl_rsa.h>

void	genrsa_gen_key(t_genrsa_args *args, t_rand_gen *rand)
{
	t_rsa_key	key;
	uint64_t	tmp;
	char		*b64;
	size_t		b64_len;

	ft_fdprintf(2, "Generating RSA private key, 64 bit long modulus (2 primes)\n");
	key.publicExponent = 65537;
	ft_fdprintf(2, "e is %lu (0x%x)\n", key.publicExponent, key.publicExponent);
	key.prime1 = gen_prime(32, rand);
	key.prime2 = gen_prime(32, rand);
	key.modulus = key.prime1 * key.prime2;
	tmp = lcm(key.prime1 - 1, key.prime2 - 1);
	gcd_ext(key.publicExponent, tmp, &key.privateExponent, NULL);
	key.privateExponent = (key.privateExponent + tmp) % tmp;
	key.exponent1 = key.privateExponent % (key.prime1 - 1);
	key.exponent2 = key.privateExponent % (key.prime2 - 1);
	key.coefficient = pow_mod(key.prime2, key.prime1 - 2, key.prime1);
	b64 = asn1_enc_b64_key(&key, &b64_len);
	ft_fdprintf(args->fd_out, "-----BEGIN RSA PRIVATE KEY-----\n");
	ft_fdprintf(args->fd_out, "%.*s\n", b64_len, b64);
	ft_fdprintf(args->fd_out, "-----END RSA PRIVATE KEY-----\n");
	free(b64);
}

int		genrsa_process(t_genrsa_args *args)
{
	t_rand_gen rand;

	args->fd_out = 1;
	if (args->filename_out != NULL && (args->fd_out = \
		open(args->filename_out, O_RDWR | O_CREAT | O_TRUNC, 0600)) == -1)
	{
		return (print_errno("Can't open output file: "));
	}
	init_rand(&rand, args->filename_rand);
	genrsa_gen_key(args, &rand);
	destroy_rand(&rand);
	close(args->fd_out);
	return (0);
}
