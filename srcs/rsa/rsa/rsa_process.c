/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 17:21:01 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/02 15:08:43 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_rsa.h>

static void	print_rsa_key(t_rsa_args *args, t_rsa_key *key)
{
	if (args->pubin)
	{
		ft_printf("RSA Public-Key: (64 bit)\nModulus: %lu (0x%x)\nExponent: %d (0x%x)\n", \
			key->modulus, key->modulus, key->publicExponent, key->publicExponent);
	}
	else
	{
		ft_printf("RSA Private-Key: (64 bit, 2 primes)\n");
		ft_printf("modulus: %lu (0x%lx)\n", key->modulus, key->modulus);
		ft_printf("publicExponent: %lu (0x%lx)\n", key->publicExponent, key->publicExponent);
		ft_printf("privateExponent: %lu (0x%lx)\n", key->privateExponent, key->privateExponent);
		ft_printf("prime1: %lu (0x%lx)\n", key->prime1, key->prime1);
		ft_printf("prime2: %lu (0x%lx)\n", key->prime2, key->prime2);
		ft_printf("exponent1: %lu (0x%lx)\n", key->exponent1, key->exponent1);
		ft_printf("exponent2: %lu (0x%lx)\n", key->exponent2, key->exponent2);
		ft_printf("coefficient: %lu (0x%lx)\n", key->coefficient, key->coefficient);
	}
}

static  int	continue_rsa_process(t_rsa_args *args, t_rsa_key *key)
{
	if (args->pubin == true && args->check == true)
		return (print_error("Only private keys can be checked"));
	if (args->text == true)
		print_rsa_key(args, key);
	return (0);
}

int			rsa_process(t_rsa_args *args)
{
	t_asn1_conf asn1_conf;
	t_rsa_key	key;
	char		*in;
	size_t		size_in;
	int			ret;

	asn1_conf.type = args->in_type;
	asn1_conf.public = args->pubin;
	if (args->filename_in)
	{
		if ((args->fd_in = open(args->filename_in, O_RDONLY)) == -1)
			return (print_errno("ft_ssl: Can't open input file: "));
		if (read_whole_file(args->fd_in, (void **)&in, &size_in) != 0)
			return (print_errno("ft_ssl: Error while reading input file: "));
		close(args->fd_in);
	}
	else if (read_whole_stdin((void **)&in, &size_in) != 0)
		return (print_errno("ft_ssl: Error while reading stdin: "));
	ret = asn1_dec_key(in, size_in, &asn1_conf, &key);
	free(in);
	return (continue_rsa_process(args, &key));
}
