/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_rsa.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 23:40:18 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/27 03:55:59 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_RSA_H
# define FT_SSL_RSA_H

#include <stdint.h>

//Names are taken from https://en.wikipedia.org/wiki/RSA_(cryptosystem)#Key_generation

typedef struct	s_rsa_key {
	uint64_t	p;
	uint64_t	q;
	uint64_t	n;
	uint64_t	lambda;
	uint64_t	exponent;
	uint64_t	d;
}				t_rsa_key;



#endif
