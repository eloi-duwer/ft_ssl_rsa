/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 19:05:56 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/20 19:00:26 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_asn1.h>
#include <ft_ssl.h>

void		write_bytes(t_buff *buff, void *bytes, size_t len)
{
	uint8_t	*tmp;
	size_t	max;

	if (len + buff->curr_len > buff->buff_len)
	{
		max = buff->buff_len * 2 > len ? buff->buff_len * 2 : len;
		if ((tmp = (uint8_t *)malloc(max)) == NULL)
			exit(print_errno("ft_ssl (malloc)"));
		ft_memcpy(tmp, buff->buff, buff->curr_len);
		buff->buff_len = max;
	}
	ft_memcpy(&buff->buff[buff->curr_len], bytes, len);
	buff->curr_len += len;
}

void		write_size(t_buff *buff, size_t n)
{
	uint8_t	b;
	uint8_t	c;

	if (n < 128)
	{
		b = 0xFF & n;
		write_bytes(buff, &b, 1);
		return ;
	}
	b = n / 256;
	write_bytes(buff, &b, 1);
	c = 0;
	while (c <= n / 256)
	{
		b = (n >> (8 * (n / 256 - c))) & 0xFF;
		write_bytes(buff, &b, 1);
		c++;
	}
}

void		write_integer(t_buff *buff, uint64_t n)
{
	uint8_t	n_bytes;
	uint8_t	tmp;
	bool	zero_pad;

	write_bytes(buff, "\x02", 1);
	if (n == 0)
		n_bytes = 0;
	else
	{
		n_bytes = 7;
		while (((n >> (8 * n_bytes)) & 0xFF) == 0)
			n_bytes--;
	}
	if ((n >> (8 * n_bytes)) & 0x80)
	{
		zero_pad = true;
		n_bytes++;
	}
	else
		zero_pad = false;
	n_bytes++;
	write_bytes(buff, &n_bytes, 1);
	if (zero_pad)
	{
		write_bytes(buff, "\x00", 1);
		n_bytes--;
	}
	while (n_bytes-- > 0)
	{
		tmp = (n >> (8 * n_bytes)) & 0xFF;
		write_bytes(buff, &tmp, 1);
	}
}

void		wrap_into_bit_string(t_buff *buff)
{
	t_buff tmp;

	init_buff(&tmp);
	write_bytes(&tmp, "\x03", 1);
	write_size(&tmp, buff->curr_len + 1);
	write_bytes(&tmp, "\x00", 1);
	write_bytes(&tmp, buff->buff, buff->curr_len);
	erase_buff(buff);
	ft_memcpy(buff, &tmp, sizeof(t_buff));
}
