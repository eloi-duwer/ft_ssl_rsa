/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_parse_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 17:33:18 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/12 14:51:44 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_hash.h>
#include <ft_ssl.h>

static int	hash_parse_subargs(t_hash_args *args, int ac, char **av)
{
	args->av_j = 0;
	while (++args->av_j < ft_strlen(av[args->av_i]))
	{
		if (av[args->av_i][args->av_j] == 'q')
			args->quiet = true;
		else if (av[args->av_i][args->av_j] == 'r')
			args->reverse = true;
		else if (av[args->av_i][args->av_j] == 'h')
			exit(print_hash_usage() && 0);
		else if (av[args->av_i][args->av_j] == 'p')
			process_stdin(args, false);
		else if (av[args->av_i][args->av_j] == 's')
			process_string(args, ac, av);
		else
		{
			ft_fdprintf(2, "Illegal option -- %c\n", \
				av[args->av_i][args->av_j]);
			return (print_hash_usage());
		}
	}
	return (0);
}

static int	hash_parse_args(t_hash_args *args, int ac, char **av)
{
	while (++args->av_i < (size_t)ac)
	{
		if (args->stop_parsing == true)
			process_file(args, av[args->av_i]);
		else if (ft_strcmp(av[args->av_i], "--") == 0)
			args->stop_parsing = true;
		else if (av[args->av_i][0] == '-')
		{
			if (hash_parse_subargs(args, ac, av) != 0)
				return (1);
		}
		else
		{
			args->stop_parsing = true;
			process_file(args, av[args->av_i]);
		}
	}
	if (args->print_stdin == true)
		process_stdin(args, true);
	return (args->return_status);
}

int			hash_arg_parsing(int ac, char **av)
{
	t_hash_args	args;

	args.av_i = 1;
	args.av_j = 0;
	args.quiet = false;
	args.reverse = false;
	args.return_status = 0;
	args.read_stdin = false;
	args.print_stdin = true;
	args.stop_parsing = false;
	if (ft_strcmp(av[1], "md5") == 0)
		args.hash_func = calc_md5;
	else if (ft_strcmp(av[1], "sha256") == 0)
		args.hash_func = calc_sha256;
	else if (ft_strcmp(av[1], "sha512") == 0)
		args.hash_func = calc_sha512;
	else if (ft_strcmp(av[1], "sha384") == 0)
		args.hash_func = calc_sha384;
	else
		return (print_commands(av[1]));
	args.func_name = av[1];
	ft_strtoupper(args.func_name);
	return (hash_parse_args(&args, ac, av));
}
