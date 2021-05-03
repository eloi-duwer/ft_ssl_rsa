/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_key.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 17:23:47 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/03 17:43:28 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_rsa.h>

static int	check_value(t_rsa_args *args, uint64_t n, uint64_t expected, char *err)
{
	if (n != expected)
	{
		ft_fdprintf(args->fd_out, "RSA key error: %s\n", err);
		return (1);
	}
	return (0);
}

/*
** Checks if the components of the key are well constructed
** Basically it reproduces the steps used to generate the key (check genrsa_gen_key)
** And checks that the values are the one expected
*/

void		check_key(t_rsa_args *args, t_rsa_key *key)
{
	t_rand_gen	r;
	int			err;

	err = 0;
	init_rand(&r, NULL);
	//prime1 prime
	err |= check_value(args, is_prime(key->prime1, 42, &r), 1, "p not prime");
	//prime2 prime
	err |= check_value(args, is_prime(key->prime2, 42, &r), 1, "q not prime");
	//modulus = prime1 * prime2
	err |= check_value(args, key->modulus, key->prime1 * key->prime2, "n does not equal p q");
	//the public exponent is not one or even
	err |= check_value(args, key->publicExponent == 1 || key->publicExponent % 2 == 0, 0, "bad e value");
	//the private key exponent is well constructed
	err |= check_value(args, mult_mod(key->privateExponent, key->publicExponent, \
		lcm(key->prime1 - 1, key->prime2 - 1)), 1, "d e not congruent to 1");
	//the exponents for the CRT are well constructed
	err |= check_value(args, key->exponent1, key->privateExponent % (key->prime1 - 1), "dmp1 not congruent to d");
	err |= check_value(args, key->exponent2, key->privateExponent % (key->prime2 - 1), "dmq1 not congruent to d");
	err |= check_value(args, key->coefficient, pow_mod(key->prime2, key->prime1 - 2, key->prime1), "iqmp not inverse of q");
	if (err == 0)
		ft_fdprintf(args->fd_out, "RSA key ok\n");
	destroy_rand(&r);
}
