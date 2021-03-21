/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dec_base64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 17:21:06 by eduwer            #+#    #+#             */
/*   Updated: 2020/12/30 17:24:10 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_base64.h>

static inline bool	is_valid(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || \
		(c >= '0' && c <= '9') || c == '+' || c == '/' || c == '=');
}

static inline void	get_next_chars(char **str, const char *end_str, char *buff)
{
	int	i;

	i = 0;
	while (i < 4 && *str < end_str)
	{
		if (is_valid(**str) == true)
		{
			buff[i] = **str;
			i++;
		}
		(*str)++;
	}
	if (i != 4)
		ft_memset(&buff[i], '=', 4 - i);
}

char				*dec_base64(char *str, size_t size, size_t *ret_size)
{
	size_t		actual_size;
	char		buff[4];
	char		*ret;
	char		*end;

	end = str + size;
	*ret_size = ((size / 4) + ((size % 4) != 0)) * 3 + 1;
	if ((ret = (char *)ft_memalloc(*ret_size)) == NULL)
		return (NULL);
	actual_size = 0;
	while (str < end)
	{
		get_next_chars(&str, end, buff);
		dec_chars(&ret[actual_size], buff, &actual_size);
	}
	*ret_size = actual_size;
	return (ret);
}
