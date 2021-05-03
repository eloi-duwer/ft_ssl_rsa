/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/31 15:23:10 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/03 18:01:15 by eduwer           ###   ########.fr       */
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

typedef	struct	s_ssl_args {
	int			i;
	int			ac;
	char		**av;
}				t_ssl_args;

typedef struct	s_rand_gen {
	int			fd;
}				t_rand_gen;

int				print_hash_usage(void);
int				print_base64_usage(void);
int				print_des_usage(void);
int				print_commands(char *name);
int				print_errno(char *str);
int				print_error(char *str);
char			*print_bits(void *bytes, size_t size);
uint64_t		reverse_bits_u64(uint64_t in);
uint32_t		reverse_bits_u32(uint32_t in);
uint64_t		swap_bits_u64(uint64_t in, const uint8_t *swap, size_t swap_len);
uint32_t		swap_bits_u32(uint32_t in, const uint8_t *swap, size_t swap_len);
uint32_t		left_rotate(uint32_t buff, unsigned int amount);
uint32_t		right_rotate(uint32_t buff, unsigned int amount);
void			xor_array(void *inout, void *b, size_t size);
int				hash_arg_parsing(int ac, char **av);
int				base64_arg_parsing(int ac, char **av);
int				des_args_parsing(int ac, char **av);
bool			is_prime(const uint64_t nb, uint64_t iter, t_rand_gen *r);
void			init_rand(t_rand_gen *r, char *filename);
void			destroy_rand(t_rand_gen *r);
void			get_rand(t_rand_gen *r, void *buff, ssize_t size);
uint64_t		get_64b_rand(t_rand_gen *r);
uint64_t		get_rand_size(t_rand_gen *r, size_t size);
char			*get_pass(char *pass_source, size_t min_len, char *ask_phrase);
char			*get_pass_stdin(char *ask_phrase);
uint64_t		gen_prime(ssize_t size, t_rand_gen *r);
uint64_t		pow_mod(uint64_t a, uint64_t b, uint64_t mod);
uint64_t		mult_mod(uint64_t a, uint64_t b, uint64_t mod);
uint64_t		add_mod(uint64_t a, uint64_t b, uint64_t mod);
uint64_t		lcm(uint64_t a, uint64_t b);
uint64_t		gcd(uint64_t a, uint64_t b);
uint64_t		gcd_ext(uint64_t a, uint64_t b, uint64_t *u, uint64_t *v);
int				rsa_args_parsing(int ac, char **av);
void			print_b64_format(char *b64, size_t b64_len, int fd, size_t n_char_line);

#endif
