/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_parse_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 18:11:11 by eduwer            #+#    #+#             */
/*   Updated: 2021/09/23 21:28:19 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_rsa.h>
#include <ft_ssl.h>

int	print_genrsa_usage(void)
{
	ft_printf("Usage: ft_ssl genrsa [options]\n\nOptions:\
\n  -i <file>\tRandom data input file\
\n  -o <file>\tOutput file\n");
	return (1);
}

int	print_rsa_usage(void)
{
	ft_printf("Usage: ft_ssl rsa [options]\n\nOptions:\
\n  -inform format\tInput format, one of DER PEM\
\n  -outform format\tOutput format, one of DER PEM\
\n  -in val\t\tInput file\
\n  -out outfile\t\tOutput file\
\n  -pubin\t\tExpect a public key in input file\
\n  -pubout\t\tOutput a public key\
\n  -passout val\t\tOutput file pass phrase source\
\n  -passin val\t\tInput file pass phrase source\
\n  -noout\t\tDon't print key out\
\n  -text\t\t\tPrint the key in text\
\n  -modulus\t\tPrint the RSA key modulus\
\n  -check\t\tVerify key consistency\
\n  -des\t\t\tEncrypt key with DES\n");
	return (1);
}

int	print_rsautl_usage(void)
{
	ft_printf("Usage: ft_ssal rsautl [options]\n\nOptions:\
\n  -in file\tInput file\
\n  -out file\tOutput file\
\n  -inkey key\tInput key file\
\n  -pubin\tInput key is an RSA public key\
\n  -encrypt\tEncrypt with public key (default)\
\n  -decrypt\tDecrypt with private key\
\n  -hexdump\tHex dump output\n");
	return (1);
}

int	rsa_args_parsing(int ac, char **av)
{
	if (ft_strcmp(av[1], "genrsa") == 0)
		return parse_genrsa_args(ac, av);
	else if (ft_strcmp(av[1], "rsa") == 0)
		return parse_rsa_args(ac, av);
	else if (ft_strcmp(av[1], "rsautl") == 0)
		return (parse_rsault_args(ac, av));
	return (print_commands(av[1]));
}
