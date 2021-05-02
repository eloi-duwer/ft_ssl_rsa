/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 13:39:15 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/02 14:04:00 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <ft_ssl_asn1.h>

uint8_t	asn1_read_byte(t_asn1_buff *buff)
{
	size_t	ret;

	if (buff->curr_len + 1 > buff->buff_len)
		exit(print_error("Unexpected end of ASN.1 buffer encountered"));
	ret = buff->buff[buff->curr_len];
	buff->curr_len++;
	return (ret);
}

size_t	asn1_read_size(t_asn1_buff *buff)
{
	size_t	len;
	size_t	tmp;

	len = asn1_read_byte(buff);
	if (len <= 0x7F)
		return (len);
	if (len == 0x80)
		return (buff->buff_len - buff->curr_len);
	len = len & 0x7F;
	tmp = 0;
	while (len > 0)
	{
		tmp = tmp << 8;
		tmp += asn1_read_byte(buff);
		len--;
	}
	return (tmp);
}

/*
** Asserts that the buffer points to a Sequence, 
** checks the length of the sequence
** And makes the buffer point to the start of the sequence
** Returns the length of the sequence
*/

size_t	asn1_read_object(t_asn1_buff *buff, uint8_t identifier)
{
	size_t	size;
	uint8_t	tmp;
	char	*err;

	if ((tmp = asn1_read_byte(buff)) != identifier)
	{
		ft_asprintf(&err, "Unexpected ASN.1 identifier: Got \\x%.02d, Expected \\x%.02d", tmp, identifier);
		print_error(err);
		free(err);
		exit(1);
	}
	size = asn1_read_size(buff);
	if (buff->curr_len + size > buff->buff_len)
		exit(print_error("ASN.1 length for sequence does not match"));
	return (size);
}

uint64_t	asn1_read_integer(t_asn1_buff *buff)
{
	uint64_t	ret;
	uint8_t		tmp;
	size_t		size;

	ret = 0;
	tmp = asn1_read_byte(buff);
	if (tmp != '\x02')
		exit(print_error("ASN.1: Unexpected type (expected Integer, \\x02)"));
	size = asn1_read_size(buff);
	tmp = asn1_read_byte(buff);
	if (size >= 10 || (size == 9 && tmp != 0))
		exit(print_error("ASN.1: Can't read integers larger than 68 bits"));
	ret = tmp;
	size--;
	while (size > 0)
	{
		ret = ret << 8;
		ret += asn1_read_byte(buff);
		size--;
	}
	return (ret);
}
