/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pass.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 17:48:28 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/20 14:05:45 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <termios.h>
#include <ft_ssl.h>

/*
** As getpass is considered obsolete, this is the
** recommended minimal implementation taken from
** https://www.gnu.org/software/libc/manual/html_node/getpass.html
*/

char	*get_pass_stdin(char *ask_phrase, bool verify_pass)
{
	struct termios	old;
	struct termios	new;
	char			*pass;
	char			*verify;
	int				fd;

	if ((fd = open("/dev/tty", O_RDWR)) == -1)
	{
		print_errno("ft_ssl: Can't open /dev/tty: ");
		return (NULL);
	}
	if (tcgetattr(fd, &old) != 0)
		exit(print_errno("Can't get password: "));
	ft_memcpy(&new, &old, sizeof(struct termios));
	new.c_lflag &= ~ECHO;
	if (tcsetattr(fd, TCSAFLUSH, &new) != 0)
		exit(print_errno("Can't get password: "));
	ft_fdprintf(fd, "%s: ", ask_phrase);
	if (get_next_line(fd, &pass) == -1)
		exit(print_errno("Can't get password: "));
	ft_fdprintf(fd, "\n");
	if (verify_pass == true)
	{
		ft_fdprintf(fd, "Verifying - %s: ", ask_phrase);
		if (get_next_line(fd, &verify) == -1)
			exit(print_errno("Can't get password: "));
		ft_fdprintf(fd, "\n");
		if (ft_strcmp(pass, verify) != 0)
		{
			ft_fdprintf(2, "Verify failure: The passwords don't match\n");
			free(verify);
			free(pass);
			tcsetattr(fd, TCSAFLUSH, &old);
			return (NULL);
		}
		free(verify);
	}
	tcsetattr(fd, TCSAFLUSH, &old);
	close(fd);
	return (pass);
}

char	*get_pass_env(char *env_var)
{
	char	*pass;

	pass = getenv(env_var);
	if (pass == NULL)
	{
		ft_fdprintf(2, "Env variable %s does not exist\n", env_var);
		return (NULL);
	}
	return (ft_strdup(pass));
}

char	*get_pass_fd(int fd)
{
	char	*pass;
	size_t	pass_len;
	char	*tmp;
	char	buff[64];
	int		ret_read;

	pass = NULL;
	pass_len = 0;
	while ((ret_read = read(fd, buff, 64)) > 0)
	{
		tmp = ft_memcat(pass, pass_len, buff, ret_read);
		free(pass);
		if (tmp == NULL)
		{
			print_errno("ft_ssl memcat: ");
			return (NULL);
		}
		pass = tmp;
		pass_len += ret_read;
	}
	if (ret_read < 0)
	{
		print_errno("ft_ssl read pass: ");
		free(pass);
		return (NULL);
	}
	return (pass);
}

char	*get_pass_file(char *filename)
{
	int		fd;
	char	*ret;

	if ((fd = open(filename, O_RDONLY)) == -1)
	{
		print_errno("ft_ssl open file: ");
		return (NULL);
	}
	ret = get_pass_fd(fd);
	close(fd);
	return (ret);
}

char	*get_pass(char *pass_source, size_t min_len, char *ask_phrase, bool verfify_pass)
{
	char	*ret;

	if (pass_source == NULL || ft_strcmp(pass_source, "stdin") == 0)
		ret = get_pass_stdin(ask_phrase, verfify_pass);
	else if (ft_strncmp(pass_source, "pass:", 5) == 0)
		ret = ft_strdup(pass_source + 5);
	else if (ft_strncmp(pass_source, "env:", 4) == 0)
		ret = get_pass_env(pass_source + 4);
	else if (ft_strncmp(pass_source, "fd:", 3) == 0)
		ret = get_pass_fd(ft_atoi(pass_source + 3));
	else if (ft_strncmp(pass_source, "file:", 5) == 0)
		ret = get_pass_file(pass_source + 5);
	else
	{
		ret = NULL;
		ft_fdprintf(2, "Invalid password argument: \"%s\"\n", pass_source);
	}
	if (ret == NULL)
	{
		ft_fdprintf(2, "Cannot get password\n");
		return (NULL);
	}
	if (ft_strlen(ret) < min_len)
	{
		ft_fdprintf(2, "Password too short: Must be at least %lu characters\n", min_len);
		return (NULL);
	}
	return (ret);
}
