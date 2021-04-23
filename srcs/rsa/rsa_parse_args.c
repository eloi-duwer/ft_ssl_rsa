/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa_parse_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 18:11:11 by eduwer            #+#    #+#             */
/*   Updated: 2021/04/23 17:23:17 by eduwer           ###   ########.fr       */
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

int	rsa_args_parsing(int ac, char **av)
{
	if (ft_strcmp(av[1], "genrsa") == 0)
		return parse_genrsa_args(ac, av);
	else if (ft_strcmp(av[1], "rsa") == 0)
		return parse_rsa_args(ac, av);
	/*else if (ft_strcmp(av[1], "rsault") == 0)
		return (parse_rsault_args(ac, av));*/
	return (print_commands(av[1]));
}
