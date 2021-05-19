#define _GNU_SOURCE
#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define N_BYTES 4

uint64_t	reverse_bits_u64(uint64_t in)
{
	int			i;
	uint64_t	ret;

	i = 0;
	ret = 0;
	while (i < 64 / 8)
	{
		ret = ret << 8;
		ret |= (in >> (i * 8)) & 0xFF;
		i++;
	}
	return (ret);
}

void write_file(uint64_t n)
{
	int fd;

	n = reverse_bits_u64(n) >> (8 * (8 - N_BYTES));
	fd = open("./file", O_WRONLY | O_TRUNC);
	write(fd, &n, N_BYTES);
	close(fd);
}

int main()
{
	//Modulus of the RSA key, we can't encrypt numbers larger than this
	uint64_t 	modulus = 1221204843900436951;
	int 		fd = open("/dev/urandom", O_RDONLY);
	uint64_t	n;
	char		n_str[N_BYTES * 4 + 1];
	uint64_t	ret;
	FILE		*file;
	int			i;
	size_t		n_tests;

	n_tests = 0;
	while (1)
	{
		read(fd, &n, N_BYTES);
		if (n > modulus)
			continue;
		i = 0;
		bzero(n_str, N_BYTES * 4 + 1);
		if (n_tests % 1000 == 0)
			printf("Test #%lld\n", n_tests);
		write_file(n);
		file = popen("./ft_ssl rsautl -inkey key -in file -out file2", "r");
		pclose(file);
		file = popen("./ft_ssl rsautl -inkey key -in file2 -decrypt", "r");
		fread(&ret, 8, 1, file);
		//Due to raw padding, the output of rsautl -decrypt will have always 8 bytes, and we read in w
		if (reverse_bits_u64(ret) != n/* << 8 * (8 - N_BYTES))*/) {
			printf("ERROR FOUND FOR INPUT NBR %llX (got %llX)\n", n, ret);
			return (1);
		}
		pclose(file);
		n_tests++;
	}
	return (0);
}