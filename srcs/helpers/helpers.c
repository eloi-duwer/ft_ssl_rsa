/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 17:52:03 by eduwer            #+#    #+#             */
/*   Updated: 2021/01/02 01:19:52 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

uint32_t	left_rotate(uint32_t buff, unsigned int amount)
{
	return (buff << amount | buff >> (32 - amount));
}

uint32_t	right_rotate(uint32_t buff, unsigned int amount)
{
	return (buff >> amount | buff << (32 - amount));
}

void		xor_array(void *inout, void *b, size_t size)
{
	unsigned char	*ptr_a;
	unsigned char	*ptr_b;

	ptr_a = (unsigned char *)inout;
	ptr_b = (unsigned char *)b;
	while (size > 0)
	{
		*ptr_a ^= *ptr_b;
		ptr_a++;
		ptr_b++;
		size--;
	}
}
