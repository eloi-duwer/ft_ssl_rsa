/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_base64.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 01:50:28 by eduwer            #+#    #+#             */
/*   Updated: 2020/12/31 01:50:29 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_BASE64_H
# define FT_SSL_BASE64_H

# include <stdbool.h>
# include <string.h>
# include <libft.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

typedef struct	s_base64_args {
	bool			decode;
	char			*input_file;
	char			*output_file;
	int				av_i;
	char			*data;
	size_t			data_size;
	int				fd;
}				t_base64_args;

char			*enc_base64(char *str, size_t size, size_t *ret_size);
char			*dec_base64(char *str, size_t size, size_t *ret_size);
char			get_enc_char_1(char *str);
char			get_enc_char_2(char *str);
char			get_enc_char_3(char *str);
char			get_enc_char_4(char *str);
void			dec_chars(char *str, char *buff, size_t *actual_size);

#endif
