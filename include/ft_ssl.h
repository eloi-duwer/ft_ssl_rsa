/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/31 15:23:10 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/21 03:34:02 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <stdlib.h>
# include <unistd.h>
# include <libft.h>
# include <stdint.h>
# include <stdbool.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>

int			print_hash_usage(void);
int			print_base64_usage(void);
int			print_des_usage(void);
int			print_commands(char *name);
int			print_errno(char *str);
int			print_error(char *str);
char		*print_bits(void *bytes, size_t size);
uint64_t	reverse_bits_u64(uint64_t in);
uint32_t	reverse_bits_u32(uint32_t in);
uint64_t	swap_bits_u64(uint64_t in, const uint8_t *swap, size_t swap_len);
uint32_t	swap_bits_u32(uint32_t in, const uint8_t *swap, size_t swap_len);
uint32_t	left_rotate(uint32_t buff, unsigned int amount);
uint32_t	right_rotate(uint32_t buff, unsigned int amount);
void		xor_array(void *inout, void *b, size_t size);
int			hash_arg_parsing(int ac, char **av);
int			base64_arg_parsing(int ac, char **av);
int			des_args_parsing(int ac, char **av);
bool		is_prime(const uint64_t nb, uint64_t iter);
uint64_t	get_rand(void);
uint64_t	pow_mod(uint64_t a, uint64_t b, uint64_t mod);

#endif
