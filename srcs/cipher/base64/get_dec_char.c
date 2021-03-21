/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dec_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 17:21:18 by eduwer            #+#    #+#             */
/*   Updated: 2020/12/30 17:31:35 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_base64.h>

/*
** Easiest way to find the corresponding value for each letter,
** Without branches
*/

static const char g_decode_base64[255] = {
	['A'] = 0,
	['B'] = 1,
	['C'] = 2,
	['D'] = 3,
	['E'] = 4,
	['F'] = 5,
	['G'] = 6,
	['H'] = 7,
	['I'] = 8,
	['J'] = 9,
	['K'] = 10,
	['L'] = 11,
	['M'] = 12,
	['N'] = 13,
	['O'] = 14,
	['P'] = 15,
	['Q'] = 16,
	['R'] = 17,
	['S'] = 18,
	['T'] = 19,
	['U'] = 20,
	['V'] = 21,
	['W'] = 22,
	['X'] = 23,
	['Y'] = 24,
	['Z'] = 25,
	['a'] = 26,
	['b'] = 27,
	['c'] = 28,
	['d'] = 29,
	['e'] = 30,
	['f'] = 31,
	['g'] = 32,
	['h'] = 33,
	['i'] = 34,
	['j'] = 35,
	['k'] = 36,
	['l'] = 37,
	['m'] = 38,
	['n'] = 39,
	['o'] = 40,
	['p'] = 41,
	['q'] = 42,
	['r'] = 43,
	['s'] = 44,
	['t'] = 45,
	['u'] = 46,
	['v'] = 47,
	['w'] = 48,
	['x'] = 49,
	['y'] = 50,
	['z'] = 51,
	['0'] = 52,
	['1'] = 53,
	['2'] = 54,
	['3'] = 55,
	['4'] = 56,
	['5'] = 57,
	['6'] = 58,
	['7'] = 59,
	['8'] = 60,
	['9'] = 61,
	['+'] = 62,
	['/'] = 63
};

static inline void	dec_char_1(char *ret, char *buff)
{
	ret[0] = g_decode_base64[(int)buff[0]] << 2;
	ret[0] |= g_decode_base64[(int)buff[1]] >> 4;
}

static inline void	dec_char_2(char *ret, char *buff)
{
	ret[1] = g_decode_base64[(int)buff[1]] << 4;
	ret[1] |= g_decode_base64[(int)buff[2]] >> 2;
}

static inline void	dec_char_3(char *ret, char *buff)
{
	ret[2] = g_decode_base64[(int)buff[2]] << 6;
	ret[2] |= g_decode_base64[(int)buff[3]];
}

void				dec_chars(char *ret, char *buff, size_t *actual_size)
{
	if (buff[0] != '=' && buff[1] != '=')
	{
		dec_char_1(ret, buff);
		(*actual_size)++;
		if (buff[2] != '=')
		{
			dec_char_2(ret, buff);
			(*actual_size)++;
			if (buff[3] != '=')
			{
				dec_char_3(ret, buff);
				(*actual_size)++;
			}
		}
	}
}
