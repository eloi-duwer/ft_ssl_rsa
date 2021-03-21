/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enc_base64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 17:21:12 by eduwer            #+#    #+#             */
/*   Updated: 2021/01/02 00:57:06 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_base64.h>

void	b64_finish_padding(char *str, size_t padding, char *ret)
{
	char buff[4];

	ft_memset(buff, '\0', 4);
	if (padding == 0)
		return ;
	ft_memcpy(buff, str, 3 - padding);
	ret[0] = get_enc_char_1(buff);
	ret[1] = get_enc_char_2(buff);
	if (padding == 1)
		ret[2] = get_enc_char_3(buff);
	ft_memset(&ret[4 - padding], '=', padding);
}

/*
** nb_groups: number of groups of 4 complete chars
** padding: number of = at the end of the string
**	by design, this number can only be 0, 1 or 2
** 	if there is padding, 4 characters will be appended
*/

char	*enc_base64(char *str, size_t size, size_t *ret_size)
{
	size_t	nb_groups;
	size_t	padding;
	size_t	i;
	char	*ret;

	nb_groups = size / 3;
	padding = (size % 3 != 0) * (3 - (size % 3));
	*ret_size = (nb_groups + (padding > 0)) * 4;
	if ((ret = (char *)ft_memalloc(*ret_size + 1)) == NULL)
		return (NULL);
	i = 0;
	while (i < nb_groups * 4)
	{
		ret[i] = get_enc_char_1(str);
		ret[i + 1] = get_enc_char_2(str);
		ret[i + 2] = get_enc_char_3(str);
		ret[i + 3] = get_enc_char_4(str);
		i += 4;
		str += 3;
	}
	b64_finish_padding(str, padding, ret + i);
	return (ret);
}
