/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_salt_key.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 03:59:35 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/18 15:13:32 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_des.h>
#include <ft_ssl.h>
#include <termios.h>

static uint64_t get_random()
{
	int			fd;
	int			ret;
	uint64_t	rng;

	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
		exit(print_errno("open"));
	ret = read(fd, &rng, 8);
	if (ret < 0)
		exit(print_errno("read"));
	close(fd);
	return rng;
}

/*
**	If decoding, salt is in the header of the file, or in argument,
**  or is not needed if we already have the key
**	If encoding, we can generate salt if none provided
*/

void	get_salt(t_des_args *ctx)
{
	uint64_t	block;

	if (ctx->decode == true)
	{
		if (ctx->no_salt == false)
		{
			des_get_next_block(ctx, &block);
			block = reverse_bits_u64(block);
			if (ft_strncmp((char *)&block, "Salted__", 8) != 0)
				exit(print_error("Error while reading: bad file format"));
			des_get_next_block(ctx, &block);
			ctx->salt = block;
		}
		else if (ctx->has_salt == false && ctx->has_key == false)
			exit(print_error("des decryption needs salt if no key is provided \
and option --no_salt is set"));
	}
	else if (ctx->has_salt == false)
		ctx->salt = get_random();
	ctx->has_salt = true;
}

/*
** As getpass is considered obsolete, this is the
** recommended minimal implementation taken from
** https://www.gnu.org/software/libc/manual/html_node/getpass.html
*/

void	get_password(t_des_args *ctx)
{
	struct termios old;
	struct termios new;

	if (tcgetattr(0, &old) != 0)
		exit(print_errno("Can't get password: "));
	ft_memcpy(&new, &old, sizeof(struct termios));
	new.c_lflag &= ~ECHO;
	if (tcsetattr(0, TCSAFLUSH, &new) != 0)
		exit(print_errno("Can't get password: "));
	ft_printf("Please enter des password: ");
	if (get_next_line(0, &ctx->password) == -1)
		exit(print_errno("Can't get password: "));
	ctx->password_malloced = true;
	ft_printf("\n");
	tcsetattr(0, TCSAFLUSH, &old);
}

void	get_key_iv(t_des_args *ctx)
{
	uint8_t		*key_iv;
	uint64_t	salt;
	int			i;

	salt = reverse_bits_u64(ctx->salt);
	if ((key_iv = pbkdf2_hmac_sha256(ctx->password, \
		(uint8_t *)&salt, 8, 16)) == NULL)
		exit(print_errno("Can't generate key: "));
	ft_memset(ctx->password, 0, ft_strlen(ctx->password));
	if (ctx->password_malloced == true)
		free(ctx->password);
	i = -1;
	while (++i < 8)
	{
		((uint8_t *)&ctx->key)[7 - i] = key_iv[i];
		if (ctx->has_iv == false)
			((uint8_t *)&ctx->iv)[7 - i] = key_iv[i + 8];
	}
	free(key_iv);
}
