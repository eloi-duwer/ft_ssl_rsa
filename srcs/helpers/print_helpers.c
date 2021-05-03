/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/02 01:19:58 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/03 17:48:16 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

/*
**	Prints the bits in the byte array, on high-endian style
*/

char		*print_bits(void *bytes, size_t size)
{
	int				i;
	size_t			j;
	char			*ret;
	unsigned char	*ptr;

	ptr = (unsigned char *)bytes;
	ret = (char *)ft_memalloc(sizeof(char) * (size * 9));
	j = 0;
	while (j < size)
	{
		i = 7;
		while (i >= 0)
		{
			ret[(j * 9) + (7 - i)] = (ptr[(size - 1) - j] & \
				(0x1 << i & 0xFF)) ? '1' : '0';
			--i;
		}
		if (j < size - 1)
			ret[j * 9 + 8] = ' ';
		++j;
	}
	return (ret);
}

int			print_errno(char *str)
{
	char	*errstr;

	errstr = strerror(errno);
	if (str != NULL && errstr != NULL)
		ft_fdprintf(2, "%s %s\n", str, errstr);
	return (1);
}

int			print_error(char *str)
{
	if (str != NULL)
		ft_fdprintf(2, "%s\n", str);
	return (1);
}

void		print_b64_format(char *b64, size_t b64_len, int fd, size_t n_char_line)
{
	size_t	i;

	i = 0;
	while (i < b64_len)
	{
		if (i + n_char_line <= b64_len)
			write(fd, b64 + i, n_char_line);
		else
			write(fd, b64 + i, b64_len - i);
		write(fd, "\n", 1);
		i += 64;
	}
}
