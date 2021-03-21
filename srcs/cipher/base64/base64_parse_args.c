/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64_parse_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 17:21:01 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/12 14:51:59 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_base64.h>
#include <ft_ssl.h>

static int	process_base64(t_base64_args *args)
{
	char	*ret;
	size_t	ret_size;
	int		fd;

	if (args->decode == true)
		ret = dec_base64(args->data, args->data_size, &ret_size);
	else
		ret = enc_base64(args->data, args->data_size, &ret_size);
	if (ret == NULL)
		return (print_errno("ft_ssl: Error during base64 process: "));
	fd = 1;
	if (args->output_file != NULL && (fd = open(args->output_file, \
		O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU)) == -1)
		return (print_errno("ft_ssl: can't open output file: "));
	if (write(fd, ret, ret_size) == -1 || \
		(args->decode == false && write(fd, "\n", 1) == -1))
		return (print_errno("ft_ssl: error while writing to file: "));
	if (fd != 1)
		close(fd);
	free(args->data);
	free(ret);
	return (0);
}

static int	read_file_and_process(t_base64_args *args)
{
	if (args->input_file != NULL)
	{
		if ((args->fd = open(args->input_file, O_RDONLY)) == -1)
			return (print_errno("ft_ssl: Can't open input file: "));
		if (read_whole_file(args->fd, \
			(void **)&args->data, &args->data_size) != 0)
			return (print_errno("ft_ssl: Error while reading input file: "));
		close(args->fd);
	}
	else if (read_whole_stdin((void **)&args->data, &args->data_size) != 0)
		return (print_errno("ft_ssl: Error while reading stdin: "));
	return (process_base64(args));
}

static int	base64_parse_subargs(t_base64_args *args, int ac, char **av)
{
	if (ft_strcmp(av[args->av_i], "-e") == 0)
		args->decode = false;
	else if (ft_strcmp(av[args->av_i], "-d") == 0)
		args->decode = true;
	else if (ft_strcmp(av[args->av_i], "-h") == 0)
		exit(print_base64_usage() && 0);
	else if (ft_strcmp(av[args->av_i], "-i") == 0)
	{
		args->av_i++;
		if (args->av_i >= ac)
			return (print_error("argument -i needs an input file after it\n"));
		args->input_file = av[args->av_i];
	}
	else if (ft_strcmp(av[args->av_i], "-o") == 0)
	{
		args->av_i++;
		if (args->av_i >= ac)
			return (print_error("argment -o needs an output file after it\n"));
		args->output_file = av[args->av_i];
	}
	else
		return (print_base64_usage());
	return (0);
}

int			base64_arg_parsing(int ac, char **av)
{
	t_base64_args args;

	ft_bzero(&args, sizeof(args));
	args.av_i = 2;
	while (args.av_i < ac)
	{
		if (av[args.av_i][0] == '-')
		{
			if (base64_parse_subargs(&args, ac, av) != 0)
				return (1);
		}
		else
		{
			ft_fdprintf(2, "Extra operand: '%s'\n", av[args.av_i]);
			return (1);
		}
		args.av_i++;
	}
	return (read_file_and_process(&args));
}
