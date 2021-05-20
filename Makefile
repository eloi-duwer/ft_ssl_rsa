# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/31 13:12:01 by eduwer            #+#    #+#              #
#    Updated: 2021/05/20 19:01:04 by eduwer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_NAME = main.c \
	helpers/helpers.c \
	helpers/print_helpers.c \
	helpers/reverse_bits.c \
	helpers/is_prime.c \
	helpers/random_generator.c \
	helpers/pow_mod.c \
	helpers/gen_prime.c \
	helpers/lcm_gcd.c \
	helpers/get_pass.c \
	helpers/buff.c \
	hash/hash_parse_args.c \
	hash/hash_process.c \
	hash/md5/md5.c \
	hash/md5/exposed_md5.c \
	hash/md5/digest.c \
	hash/md5/rounds.c \
	hash/sha256/sha256.c \
	hash/sha256/exposed_sha256.c \
	hash/sha256/sha256_helpers.c \
	hash/sha512/sha512.c \
	hash/sha512/sha512_helpers.c \
	hash/sha384/sha384.c \
	cipher/base64/enc_base64.c \
	cipher/base64/dec_base64.c \
	cipher/base64/base64_parse_args.c \
	cipher/base64/get_enc_char.c \
	cipher/base64/get_dec_char.c \
	cipher/pbkdf2/pbkdf2.c \
	cipher/pbkdf2/hmac/hmac_sha256.c \
	cipher/pbkdf2/openssl_default_key_derivation.c \
	cipher/des/des_parse_args.c \
	cipher/des/des_process.c \
	cipher/des/get_salt_key.c \
	cipher/des/subkeys.c \
	cipher/des/s_boxes.c \
	cipher/des/des_write.c \
	cipher/des/des_alg.c \
	cipher/des/read/bufferization.c \
	cipher/des/read/des_read.c \
	rsa/genrsa/genrsa.c \
	rsa/genrsa/parse_genrsa_args.c \
	rsa/asn/enc/asn1_enc.c \
	rsa/asn/enc/helpers.c \
	rsa/asn/enc/asn1_enc_priv_key.c \
	rsa/asn/dec/asn1_dec.c \
	rsa/asn/dec/helpers.c \
	rsa/asn/dec/asn1_dec_priv_key.c \
	rsa/rsa_parse_args.c \
	rsa/rsa/parse_rsa_args.c \
	rsa/rsa/rsa_process.c \
	rsa/rsa/check_key.c \
	rsa/rsautl/parse_rsautl_args.c \
	rsa/rsautl/rsautl_process.c

NAME = ft_ssl

CC = clang

CFLAGS = -I./include -I./libft/include -Wall -Wextra -Wno-unused-result -g

SRCF = ./srcs/

OBJF = ./obj/

OBJS = $(addprefix $(OBJF), $(SRC_NAME:.c=.o))

LFT = libft/libft.a

all: $(NAME)

$(NAME): $(LFT) $(OBJS)
	$(CC) -o $(NAME) $(OBJS) -L./libft -lft -g

$(LFT):
	make -C ./libft

$(OBJF)%.o: $(SRCF)%.c
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $(addprefix $(SRCF), $*.c)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
