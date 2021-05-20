/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buff.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 19:00:11 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/20 19:00:21 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

void		init_buff(t_buff *buff)
{
	buff->curr_len = 0;
	buff->buff_len = 512;
	if ((buff->buff = (uint8_t *)malloc(buff->buff_len)) == NULL)
		exit(print_errno("ft_ssl (malloc)"));
}

void		erase_buff(t_buff *buff)
{
	if (buff->buff != NULL)
	{
		ft_bzero(buff->buff, buff->buff_len);
		free(buff->buff);
	}
	buff->buff_len = 0;
	buff->curr_len = 0;
}