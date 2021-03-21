/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_hash.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 01:50:35 by eduwer            #+#    #+#             */
/*   Updated: 2021/01/15 15:37:17 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_HASH_H
# define FT_SSL_HASH_H

# include <stdbool.h>
# include <string.h>
# include <libft.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

# define A 0
# define B 1
# define C 2
# define D 3
# define E 4
# define F 5
# define G 6
# define H 7
# define UINT128 __uint128_t

typedef struct	s_md5_ctx {
	unsigned char	*message;
	size_t			original_size;
	size_t			current_size;
	uint32_t		buffera;
	uint32_t		bufferb;
	uint32_t		bufferc;
	uint32_t		bufferd;
	uint32_t		*buffers[4];
	uint32_t		savea;
	uint32_t		saveb;
	uint32_t		savec;
	uint32_t		saved;

}				t_md5_ctx;

typedef struct	s_sha256_ctx {
	unsigned char	*message;
	size_t			original_size;
	size_t			current_size;
	uint32_t		hash[8];
	uint32_t		work_var[8];
	uint32_t		work_table[64];
}				t_sha256_ctx;

typedef struct	s_sha512_ctx {
	unsigned char	*message;
	size_t			original_size;
	size_t			current_size;
	uint64_t		hash[8];
	uint64_t		work_var[8];
	uint64_t		work_table[80];
}				t_sha512_ctx;

typedef	struct	s_hash_args {
	bool			quiet;
	bool			reverse;
	int				return_status;
	bool			read_stdin;
	bool			print_stdin;
	bool			stop_parsing;
	size_t			av_i;
	size_t			av_j;
	char			*func_name;
	char			*(*hash_func) (char *input, size_t size);
}				t_hash_args;

extern uint32_t	(*g_bitwise_operation[4]) (uint32_t x, uint32_t y, uint32_t z);

char			*calc_md5(char *str, size_t size);
char			*calc_sha256(char *str, size_t size);
char			*calc_sha512(char *str, size_t size);
char			*calc_sha384(char *str, size_t size);

bool			sha256(t_sha256_ctx *ctx, char *str, size_t size);
uint8_t			*raw_sha256(uint8_t *str, size_t size);
void			process_stdin(t_hash_args *args, bool print_stdin);
void			process_string(t_hash_args *args, int ac, char **av);
void			process_file(t_hash_args *args, char *file_name);
void			universal_md5_round(t_md5_ctx *ctx, int i, uint32_t buff[16]);
uint32_t		sha256_init(char i, uint32_t word);
uint32_t		sha256_s(char i, uint32_t word);
uint32_t		sha256_ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t		sha256_maj(uint32_t x, uint32_t y, uint32_t z);
uint64_t		sha512_init(char i, uint64_t x);
uint64_t		sha512_s(char i, uint64_t x);
uint64_t		sha512_ch(uint64_t x, uint64_t y, uint64_t z);
uint64_t		sha512_maj(uint64_t x, uint64_t y, uint64_t z);
void			sha512_loop(t_sha512_ctx *ctx, int i);
int				padding_sha512(t_sha512_ctx *ctx);

#endif
