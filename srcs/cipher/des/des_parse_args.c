/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_parse_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/03 01:05:52 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/12 14:58:27 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <ft_ssl_des.h>

static int	get_arg(t_des_args *args, char **pt, char *errstr)
{
	args->av_i++;
	if (args->av_i >= args->ac)
	{
		ft_fdprintf(2, "Argument %s after it\n", errstr);
		return (1);
	}
	*pt = args->av[args->av_i];
	return (0);
}

static int	continue_des_parse_subargs(t_des_args *args)
{
	if (ft_strcmp(args->av[args->av_i], "-k") == 0)
	{
		if (get_arg(args, &args->key_str, "-k needs a hex key") != 0)
			return (1);
	}
	else if (ft_strcmp(args->av[args->av_i], "-p") == 0)
	{
		if (get_arg(args, &args->password, "-p needs a password") != 0)
			return (1);
	}
	else if (ft_strcmp(args->av[args->av_i], "-s") == 0)
	{
		if (get_arg(args, &args->salt_str, "-s needs an hex salt") != 0)
			return (1);
	}
	else if (ft_strcmp(args->av[args->av_i], "-v") == 0)
	{
		if (get_arg(args, &args->iv_str, "-v needs an hex iv") != 0)
			return (1);
	}
	else
		return (print_des_usage());
	return (0);
}

static int	des_parse_subargs(t_des_args *args)
{
	if (ft_strcmp(args->av[args->av_i], "-e") == 0)
		args->decode = false;
	else if (ft_strcmp(args->av[args->av_i], "-h") == 0)
		exit(print_des_usage() && 0);
	else if (ft_strcmp(args->av[args->av_i], "-d") == 0)
		args->decode = true;
	else if (ft_strcmp(args->av[args->av_i], "--print_keys") == 0)
		args->print_keys = true;
	else if (ft_strcmp(args->av[args->av_i], "-a") == 0)
		args->base64 = true;
	else if (ft_strcmp(args->av[args->av_i], "--no_salt") == 0)
		args->no_salt = true;
	else if (ft_strcmp(args->av[args->av_i], "-i") == 0)
	{
		if (get_arg(args, &args->filename_in, "-i needs an input file") != 0)
			return (1);
	}
	else if (ft_strcmp(args->av[args->av_i], "-o") == 0)
	{
		if (get_arg(args, &args->filename_out, "-o needs an output file") != 0)
			return (1);
	}
	else
		return (continue_des_parse_subargs(args));
	return (0);
}

static int	get_des_alg(t_des_args *args, char *str)
{
	if (ft_strcmp(str, "des-cbc") == 0)
		args->alg = cbc;
	else if (ft_strcmp(str, "des-ecb") == 0)
		args->alg = ecb;
	else
		args->alg = cbc;
	return (0);
}

int			des_args_parsing(int ac, char **av)
{
	t_des_args	args;

	ft_bzero(&args, sizeof(t_des_args));
	args.ac = ac;
	args.av = av;
	args.av_i = 2;
	if (get_des_alg(&args, av[1]) != 0)
		return (-1);
	while (args.av_i < ac)
	{
		if (av[args.av_i][0] == '-')
		{
			if (des_parse_subargs(&args) != 0)
				return (1);
		}
		else
		{
			ft_fdprintf(2, "Extra operand: '%s'\n", av[args.av_i]);
			return (1);
		}
		args.av_i++;
	}
	return (des_process(&args));
}
