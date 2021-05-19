/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 17:21:01 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/19 00:51:11 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_rsa.h>

static int	get_nb_bits(uint64_t n)
{
	int	ret;

	if (n == 0)
		return (1);
	ret = 64;
	while ((n & ((uint64_t)1 << (ret - 1))) == 0)
		ret--;
	return (ret);
}

static void	print_rsa_values(t_rsa_args *args, t_rsa_key *key)
{
	if (args->pubin)
	{
		ft_fdprintf(args->fd_out, "RSA Public-Key: (%d bit)\nModulus: %lu (0x%lx)\nExponent: %d (0x%lx)\n", \
			get_nb_bits(key->modulus), key->modulus, key->modulus, key->publicExponent, key->publicExponent);
	}
	else
	{
		ft_fdprintf(args->fd_out, "RSA Private-Key: (%d bit, 2 primes)\n", get_nb_bits(key->modulus));
		ft_fdprintf(args->fd_out, "modulus: %llu (0x%llx)\n", key->modulus, key->modulus);
		ft_fdprintf(args->fd_out, "publicExponent: %llu (0x%llx)\n", key->publicExponent, key->publicExponent);
		ft_fdprintf(args->fd_out, "privateExponent: %llu (0x%llx)\n", key->privateExponent, key->privateExponent);
		ft_fdprintf(args->fd_out, "prime1: %llu (0x%llx)\n", key->prime1, key->prime1);
		ft_fdprintf(args->fd_out, "prime2: %llu (0x%llx)\n", key->prime2, key->prime2);
		ft_fdprintf(args->fd_out, "exponent1: %llu (0x%llx)\n", key->exponent1, key->exponent1);
		ft_fdprintf(args->fd_out, "exponent2: %llu (0x%llx)\n", key->exponent2, key->exponent2);
		ft_fdprintf(args->fd_out, "coefficient: %llu (0x%llx)\n", key->coefficient, key->coefficient);
	}
}

static void	write_the_key(t_rsa_args *args, char *enc, size_t enc_len, t_asn1_conf *conf)
{
	if (args->out_type == DER)
		write(args->fd_out, enc, enc_len);
	else
	{
		if (args->pubout == true)
			write(args->fd_out, g_public_header, ft_strlen(g_public_header));
		else
		{
			write(args->fd_out, g_private_header, ft_strlen(g_private_header));
			if (args->des == true)
			{
				write(args->fd_out, g_encrypted_str, ft_strlen(g_encrypted_str));
				ft_fdprintf(args->fd_out, "%.16llX\n\n", conf->out_salt);
			}
		}
		print_b64_format(enc, enc_len, args->fd_out, 64);
		if (args->pubout == true)
			write(args->fd_out, g_public_footer, ft_strlen(g_public_footer));
		else
			write(args->fd_out, g_private_footer, ft_strlen(g_private_footer));
	}
}

static void	write_rsa_key(t_rsa_args *args, t_rsa_key *key)
{
	char		*enc;
	size_t		enc_len;
	t_asn1_conf	conf;

	ft_fdprintf(2, "writing RSA key\n");
	conf.public = args->pubout;
	conf.des = args->des;
	conf.type = args->out_type;
	conf.pass = args->passout;
	enc = asn1_enc_key(key, &conf, &enc_len);
	write_the_key(args, enc, enc_len, &conf);
	free(enc);
}

static int	continue_rsa_process(t_rsa_args *args, t_rsa_key *key)
{
	if (args->pubin == true && args->check == true)
		return (print_error("Only private keys can be checked"));
	args->fd_out = 1;
	if (args->filename_out != NULL && (args->fd_out = \
		open(args->filename_out, O_RDWR | O_CREAT | O_TRUNC, 0600)) == -1)
		return (print_errno("Can't open output file: "));
	if (args->text == true)
		print_rsa_values(args, key);
	if (args->modulus == true)
		ft_fdprintf(args->fd_out, "Modulus=%llX\n", key->modulus);
	if (args->check == true)
		check_key(args, key);
	if (args->noout == false)
		write_rsa_key(args, key);
	if (args->fd_out != 1)
		close(args->fd_out);
	return (0);
}

int			rsa_process(t_rsa_args *args)
{
	t_asn1_conf asn1_conf;
	t_rsa_key	key;
	char		*in;
	size_t		size_in;

	asn1_conf.type = args->in_type;
	asn1_conf.public = args->pubin;
	asn1_conf.pass = args->passin;
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
	if (asn1_dec_key(in, size_in, &asn1_conf, &key) != 0)
		return (1);
	free(in);
	return (continue_rsa_process(args, &key));
}
