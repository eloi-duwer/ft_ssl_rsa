/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_genrsa_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 18:43:00 by eduwer            #+#    #+#             */
/*   Updated: 2021/04/23 17:31:13 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_rsa.h>
#include <ft_ssl.h>

int			get_arg(t_ssl_args *a, char **pt, char *errstr)
{
	a->i++;
	if (a->i >= a->ac)
	{
		ft_fdprintf(2, "Argument %s after it\n", errstr);
		return (1);
	}
	*pt = a->av[a->i];
	return (0);
}

static int	genrsa_parse_subargs(t_genrsa_args *args, t_ssl_args *a)
{
	if (ft_strcmp(a->av[a->i], "-i") == 0)
	{
		if (get_arg(a, &args->filename_rand, "-i needs an input file") != 0)
			return (1);
	}
	else if (ft_strcmp(a->av[a->i], "-o") == 0)
	{
		if (get_arg(a, &args->filename_out, "-o needs an output file") != 0)
			return (1);
	}
	else
		return (print_genrsa_usage());
	return (0);
}

int			parse_genrsa_args(int ac, char **av)
{
	t_genrsa_args	args;
	t_ssl_args		a;

	ft_bzero(&args, sizeof(t_genrsa_args));
	a.i = 2;
	a.ac = ac;
	a.av = av;
	while (a.i < a.ac)
	{
		if (a.av[a.i][0] == '-')
		{
			if (genrsa_parse_subargs(&args, &a) != 0)
				return (1);
		}
		else
		{
			ft_fdprintf(2, "Extra operand: '%s'\n", a.av[a.i]);
			return (1);
		}
		a.i++;
	}
	return (genrsa_process(&args));
}
