/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsautl_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 18:26:30 by eduwer            #+#    #+#             */
/*   Updated: 2021/10/02 19:03:47 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_rsa.h>

static int		hexdump_data(int fd, uint64_t msg)
{
	unsigned char	dump[8];
	int		i;

	i = 0;
	ft_fdprintf(fd, "0000 -");
	while (i < 8)
	{
		dump[i] = msg & 0xFF;
		ft_fdprintf(fd, " %.02x", dump[i]);
		msg >>= 8;
		i++;
	}
	ft_fdprintf(fd, "-                          ");
	i = 0;
	while (i < 8)
	{
		if (ft_isprint(dump[i]))
			ft_fdprintf(fd, "%c", dump[i]);
		else
			ft_fdprintf(fd, ".");
		i++;
	}
	ft_fdprintf(fd, "\n");
	if (fd != 0)
		close(fd);
	return (0);
}

static int		print_data(t_rsautl_args *args, uint64_t msg)
{
	int		fd;
	int		i;
	uint8_t	c;

	fd = 1;
	if (args->out_file && (fd = open(args->out_file, O_RDWR | O_CREAT | O_TRUNC, 0600)) == -1)
		return (print_errno("ft_ssl: Can't open output file: "));
	msg = reverse_bits_u64(msg);
	i = 0;
	if (args->hexdump == true)
		return (hexdump_data(fd, msg));
	while (i < 8)
	{
		c = msg & 0xFF;
		write(fd, &c, 1);
		msg >>= 8;
		i++;
	}
	if (fd != 0)
		close(fd);
	return (0);
}

/*
** Decrypt using the Chinese Remainder Theorem (cf Wikipedia's page on RSA)
** Unfortunately we are working with unsigned numbers,
** So m1 - m2 must not be negative
** Fortunately we then do a modulo prime1 on this substraction, so in the case it happens
** we add prime1 enough time so that the result of m1 - m2 falls between 0 and prime1:
** m1 + key->prime1 * (m2 / key->prime1 + 1) 
*/

static uint64_t	decrypt_msg(t_rsa_key *key, uint64_t msg)
{
	uint64_t	m1;
	uint64_t	m2;
	uint64_t	h;

	m1 = pow_mod(msg, key->exponent1, key->prime1);
	m2 = pow_mod(msg, key->exponent2, key->prime2);
	if (m1 > m2)
		h = m1 - m2;
	else
		h = (m1 + key->prime1 * (m2 / key->prime1 + 1)) - m2;
	h = mult_mod(key->coefficient, h, key->prime1);
	return (m2 + (h * key->prime2));
}

static int  	continue_rsautl_process(t_rsautl_args *args, t_rsa_key *key, t_buff *msg_buff)
{
	uint64_t	msg;
	size_t		i;

	if (msg_buff->buff_len > 8)
	{
		free(msg_buff->buff);
		return (print_error("Data greater than mod len"));
	}
	msg = 0;
	i = 0;
	while (i < msg_buff->buff_len)
	{
		msg <<= 8;
		msg |= (uint64_t)msg_buff->buff[i];
		i++;
	}
	free(msg_buff->buff);
	if (msg > key->modulus)
	{
		ft_printf("msg = %llX, mod = %llX\n", msg, key->modulus);
		return (print_error("Data greater than mod len"));
	}
	if (args->decrypt == false)
		msg = pow_mod(msg, key->publicExponent, key->modulus);
	else
		msg = decrypt_msg(key, msg);
	return print_data(args, msg);
}

int 			rsautl_process(t_rsautl_args *args)
{
	t_buff		in_key;
	t_buff		msg;
	t_rsa_key   key;
	t_asn1_conf asn1_conf;
	int			fd;
	
	asn1_conf.public = args->pubin;
	asn1_conf.type = PEM;
	asn1_conf.pass = NULL;
	if ((fd = open(args->inkey_file, O_RDONLY)) == -1)
		return (print_errno("ft_ssl: Can't open input file: "));
	if (read_whole_file(fd, (void **)&in_key.buff, &in_key.buff_len) != 0)
		return (print_errno("ft_ssl: Error while reading input file: "));
	close(fd);
	if (asn1_dec_key((char *)in_key.buff, in_key.buff_len, &asn1_conf, &key) != 0)
		return (1);
	free(in_key.buff);
	if (args->in_file != NULL)
	{
		if ((fd = open(args->in_file, O_RDONLY)) == -1)
			return (print_errno("ft_ssl: Can't open input file: "));
		if (read_whole_file(fd, (void **)&msg.buff, &msg.buff_len) != 0)
			return (print_errno("ft_ssl: Error while reading input file: "));
		close(fd);
	}
	else
	{
		if (read_whole_stdin((void **)&msg.buff, &msg.buff_len) != 0)
			return (print_errno("ft_ssl: Error while reading data from stdin: "));
	}
	return (continue_rsautl_process(args, &key, &msg));
}
