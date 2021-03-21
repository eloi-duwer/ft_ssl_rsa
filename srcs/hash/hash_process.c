/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 18:16:21 by eduwer            #+#    #+#             */
/*   Updated: 2021/01/08 22:23:10 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_hash.h>
#include <ft_ssl.h>

void		process_stdin(t_hash_args *args, bool print_stdin)
{
	char	*file;
	char	*ret;
	size_t	size;

	args->print_stdin = false;
	if ((read_whole_stdin((void **)&file, &size)) != 0)
	{
		ft_fdprintf(2, "Error while reading from stdin\n");
		return ;
	}
	ret = args->hash_func(file, size);
	if (print_stdin == false)
		write(1, file, size);
	ft_printf("%s\n", ret);
	free(file);
	free(ret);
}

static char	*open_and_read_file(t_hash_args *args, \
	char *file_name, size_t *size)
{
	int		fd;
	char	*file;
	char	*err;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		ft_asprintf(&err, "ft_ssl: can't open %s: ", file_name);
		print_errno(err);
		free(err);
		args->return_status = 1;
		return (NULL);
	}
	if ((read_whole_file(fd, (void **)&file, size)) != 0)
	{
		ft_fdprintf(2, "Error while reading the file %s\n", file_name);
		close(fd);
		args->return_status = 1;
		return (NULL);
	}
	close(fd);
	return (file);
}

void		process_file(t_hash_args *args, char *file_name)
{
	char	*file;
	char	*ret;
	size_t	size;

	args->print_stdin = false;
	if ((file = open_and_read_file(args, file_name, &size)) == NULL)
		return ;
	ret = args->hash_func(file, size);
	if (args->quiet == true)
		ft_printf("%s\n", ret);
	else if (args->reverse == true)
		ft_printf("%s %s\n", ret, file_name);
	else
		ft_printf("%s (%s) = %s\n", args->func_name, file_name, ret);
	free(ret);
	free(file);
}

static void	real_process_string(t_hash_args *args, char *str)
{
	char	*ret;

	ret = args->hash_func(str, ft_strlen(str));
	if (args->quiet == true)
		ft_printf("%s\n", ret);
	else if (args->reverse == true)
		ft_printf("%s \"%s\"\n", ret, str);
	else
		ft_printf("%s (\"%s\") = %s\n", args->func_name, str, ret);
	free(ret);
}

void		process_string(t_hash_args *args, int ac, char **av)
{
	char	*err;
	size_t	len;

	args->print_stdin = false;
	if (av[args->av_i][args->av_j + 1] != '\0')
		real_process_string(args, &av[args->av_i][args->av_j + 1]);
	else if (args->av_i + 1 < (size_t)ac)
	{
		real_process_string(args, av[args->av_i + 1]);
		++args->av_i;
	}
	else
	{
		len = ft_asprintf(&err, "ft_ssl: option -s requires an argument\n");
		write(2, err, len);
		free(err);
		args->return_status = 1;
	}
	args->av_j = ft_strlen(av[args->av_i]);
}
