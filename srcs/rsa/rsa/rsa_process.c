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

int		rsa_process(t_rsa_args *args)
{
	t_asn1_conf asn1_conf;
	t_rsa_key	key;
	char		*in;
	size_t		size_in;

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
	asn1_dec_key(in, size_in, &asn1_conf, &key);
	return (0);
}
