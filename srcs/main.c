/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 14:03:45 by eduwer            #+#    #+#             */
/*   Updated: 2021/04/09 14:53:31 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

int			print_commands(char *name)
{
	if (name != NULL)
		ft_printf("ft_ssl: Error: '%s' is an invalid command.\n\n", name);
	else
		ft_printf("Usage: ./ft_ssl <command>\n\n");
	ft_printf("Standart commands:\n\n");
	ft_printf("Message Digest commands:\nmd5\nsha256\nsha384\nsha512\n\n");
	ft_printf("Cipher commands:\nbase64\ndes\ndes-cbc\ndes-ecb\n");
	return (1);
}

int			print_hash_usage(void)
{
	ft_printf("Usage: ft_ssl <hash command> [options] [files ...]\n\nOptions:\
\n  -h\t\tPrint this help and exit\
\n  -p\t\tRead from stdin, echo stdin then output the hash\
\n  -q\t\tQuiet mode, output only the hash\
\n  -r\t\tReverse mode (output the hash and then the name of the file hashed)\
\n  -s <string>\tHash the provided string\n");
	return (1);
}

int			print_base64_usage(void)
{
	ft_printf("Usage: ft_ssl base64 [options]\n\nOptions:\
\n  -d\t\tUse decrypt mode\
\n  -e\t\tUse encrypt mode (default)\
\n  -h\t\tPrint this help and exit\
\n  -i <file>\tInput file\
\n  -o <file>\tOutput file\n");
	return (1);
}

int			print_des_usage(void)
{
	ft_printf("Usage: ft_ssl [des des-ecb des-cbc] [options]\n\n\
Options:\
\n  -a\t\tThe input / output is base64 encoded\
\n  -d\t\tUse decrypt mode\
\n  -e\t\tUse encrypt mode (default)\
\n  -h\t\tPrint this help and exit\
\n  --print_keys\tPrint the salt / key / iv to stderr\
\n  --no_salt\tOn encryption: do not output the salt to the file, \
\n\t\tOn decryption: do not expect salt at the beginning of the file \
(the salt must be provided if the key is not provided)\
\n  -i <file>\tInput file\
\n  -o <file>\tOutput file\
\n  -p <password>\tPassword to use\
\n  -k <key>\tEncryption key, in hexadecimal format\
\n  -s <salt>\tSalt used for key derivation, in hexadecimal format\
\n  -v <iv>\tInitialisation vector, used for all des modes except ecb, \
in hexadecimal format\n");
	return (1);
}

int			main(int argc, char **argv)
{
	key_gen();
	/*if (argc == 1)
		return (print_commands(NULL));
	if (ft_strcmp(argv[1], "md5") == 0
		|| ft_strcmp(argv[1], "sha256") == 0
		|| ft_strcmp(argv[1], "sha512") == 0
		|| ft_strcmp(argv[1], "sha384") == 0)
		return (hash_arg_parsing(argc, argv));
	else if (ft_strcmp(argv[1], "base64") == 0)
		return (base64_arg_parsing(argc, argv));
	else if (ft_strncmp(argv[1], "des", 3) == 0)
		return (des_args_parsing(argc, argv));
	else
		return (print_commands(argv[1]));*/
}
