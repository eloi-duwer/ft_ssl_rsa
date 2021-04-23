/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rsa_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 17:16:37 by eduwer            #+#    #+#             */
/*   Updated: 2021/04/23 17:28:14 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_rsa.h>
#include <ft_ssl.h>

int	parse_rsa_args(int ac, char **av)
{
	t_rsa_args	args;
	t_ssl_args	a;

	ft_bzero(&args, sizeof(args));
	a.i = 2;
	a.ac = ac;
	a.av = av;
	while (a.i < a.ac)
	{
		if (a.av[a.i][0] == '-')
		{
			if (rsa_parse_subargs(&args, &a) != 0)
				return (1);
		}
		else
		{
			ft_fdprintf(2, "Extra operand: '%s'\n", a.av[a.i]);
			return (1);
		}
		a.i++;
	}
	return (rsa_process(&args));
}
