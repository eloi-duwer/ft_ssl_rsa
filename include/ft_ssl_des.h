/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_des.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 01:50:32 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/20 21:11:35 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_DES_H
# define FT_SSL_DES_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <libft.h>
# include <unistd.h>

typedef enum	e_des_alg
{
	ecb,
	cbc
}				t_des_alg;

/*
**	MUST be a multiple of 3, as the buffers will be used to stock data BEFORE
** 	base64 conversion, if needed. base64 encodes characters by groups of 3, if
**	it is not a multiple of 3 padding characters (= or ==) can be written in the
**	Middle of the file, and it's invalid
*/

# define DES_BUFF_SIZE 4608

typedef struct	s_des_args {
	int			ac;
	char		**av;
	int			av_i;
	t_des_alg	alg;
	bool		base64;
	bool		decode;
	char		*filename_in;
	int			fd_in;
	char		*str_in;
	size_t		strlen_in;
	size_t		pt_str_in;
	char		*str_out;
	size_t		strlen_out;
	bool		next_block_is_last;
	uint64_t	n_bl;
	bool		finished_reading;
	char		*filename_out;
	int			fd_out;
	bool		password_malloced;
	bool		no_salt;
	bool		print_keys;
	bool		use_default_keygen;
	char		*password;
	char		*key_str;
	char		*salt_str;
	char		*iv_str;
	bool		has_key;
	uint64_t	key;
	bool		has_salt;
	uint64_t	salt;
	bool		has_iv;
	uint64_t	iv;
	uint64_t	subkeys[16];
	uint8_t		r_buff[DES_BUFF_SIZE];
	size_t		r_buff_size;
	size_t		r_buff_used;
	uint8_t		w_buff[DES_BUFF_SIZE];
	size_t		w_buff_size;
	void		(*write_func)(struct s_des_args *, uint64_t);
	void		(*empty_buffer_func)(struct s_des_args *);
	void		(*write_salt_func)(struct s_des_args *);
}				t_des_args;

uint8_t			*pbkdf2_hmac_sha256(char *password, uint8_t *salt, \
					size_t salt_len, size_t dk_len_bytes);
uint8_t			*openssl_kdf(uint8_t *(*hash_func)(uint8_t *, size_t), size_t hash_len, \
				char *password, uint8_t *salt, size_t salt_len, size_t target_len);
uint8_t			*hmac_sha256(char *password, uint8_t *msg, size_t msg_len);
void			gen_subkeys(t_des_args *ctx);
uint32_t		s_boxes(uint64_t n);
void			get_salt(t_des_args *ctx);
void			get_key_iv(t_des_args *ctx);
int				des_process(t_des_args *args);
void			des_write_to_file(t_des_args *ctx, uint64_t block);
void			des_write_salt_to_file(t_des_args *ctx);
void			des_empty_buffer(t_des_args *ctx);
int				des_loop_blocks(t_des_args *ctx);
bool			des_get_next_block(t_des_args *ctx, uint64_t *bl);
void			des_init_next_block(t_des_args *ctx);
ssize_t			des_get_64_bits(t_des_args *ctx, uint64_t *bl);

#endif
