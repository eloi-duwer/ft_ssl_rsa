/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rsa_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 17:16:37 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/02 15:11:25 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_rsa.h>
#include <ft_ssl.h>

static int	rsa_parse_in_outform(t_ssl_args *a, enum e_type *type)
{
	char *str;

	if (get_arg(a, &str, "-inform and -outform needs a input format argument") != 0)
		return (1);
	if (ft_strcmp(str, "PEM") == 0)
		*type = PEM;
	else if (ft_strcmp(str, "DER") == 0)
		*type = DER;
	else
		return (print_error("Valid values for inform and outform are PEM and DER"));
	return (0);
}

static int	rsa_parse_subargs(t_rsa_args *args, t_ssl_args *a)
{
	if (ft_strcmp(a->av[a->i], "-des") == 0)
		args->des = true;
	else if (ft_strcmp(a->av[a->i], "-text") == 0)
		args->text = true;
	else if (ft_strcmp(a->av[a->i], "-noout") == 0)
		args->noout = true;
	else if (ft_strcmp(a->av[a->i], "-modulus") == 0)
		args->modulus = true;
	else if (ft_strcmp(a->av[a->i], "-check") == 0)
		args->check = true;
	else if (ft_strcmp(a->av[a->i], "-pubin") == 0)
	{
		args->pubin = true;
		args->pubout = true;
	}
	else if (ft_strcmp(a->av[a->i], "-pubout") == 0)
		args->pubout = true;
	else if (ft_strcmp(a->av[a->i], "-in") == 0)
	{
		if (get_arg(a, &args->filename_in, "-in needs an input file") != 0)
			return (1);
	}
	else if (ft_strcmp(a->av[a->i], "-out") == 0)
	{
		if (get_arg(a, &args->filename_out, "-out needs an output file") != 0)
			return (1);
	}
	else if (ft_strcmp(a->av[a->i], "-passin") == 0)
	{
		if (get_arg(a, &args->passin, "-passin needs an input password argument") != 0)
			return (1);
	}
	else if (ft_strcmp(a->av[a->i], "-passout") == 0)
	{
		if (get_arg(a, &args->passout, "passout needs an output password argument") != 0)
			return (1);
	}
	else if (ft_strcmp(a->av[a->i], "-inform") == 0)
	{
		if (rsa_parse_in_outform(a, &args->in_type) != 0)
			return (1);
	}
	else if (ft_strcmp(a->av[a->i], "-outform") == 0)
	{
		if (rsa_parse_in_outform(a, &args->out_type) != 0)
			return (1);
	}
	else
		return (print_rsa_usage());
	return (0);
}

int			parse_rsa_args(int ac, char **av)
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
