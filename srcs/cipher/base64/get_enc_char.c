/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_enc_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 17:21:23 by eduwer            #+#    #+#             */
/*   Updated: 2020/12/30 17:33:04 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_base64.h>

static const char *g_encode_base64 = \
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

inline char	get_enc_char_1(char *str)
{
	return (g_encode_base64[(str[0] & 0b11111100) >> 2]);
}

inline char	get_enc_char_2(char *str)
{
	return (g_encode_base64[((str[0] & 0b00000011) << 4) | \
		((str[1] & 0b11110000) >> 4)]);
}

inline char	get_enc_char_3(char *str)
{
	return (g_encode_base64[((str[1] & 0b00001111) << 2) | \
		((str[2] & 0b11000000) >> 6)]);
}

inline char	get_enc_char_4(char *str)
{
	return (g_encode_base64[str[2] & 0b00111111]);
}
