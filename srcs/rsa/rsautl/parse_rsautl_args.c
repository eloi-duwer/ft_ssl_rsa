/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rsautl_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 18:05:33 by eduwer            #+#    #+#             */
/*   Updated: 2021/10/02 19:05:03 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_rsa.h>

int rsautl_parse_subargs(t_rsautl_args *args, t_ac_av *a)
{
    if (ft_strcmp(a->av[a->i], "-pubin") == 0)
        args->pubin = true;
    else if (ft_strcmp(a->av[a->i], "-encrypt") == 0)
        args->decrypt = false;
    else if (ft_strcmp(a->av[a->i], "-decrypt") == 0)
        args->decrypt = true;
    else if (ft_strcmp(a->av[a->i], "-hexdump") == 0)
        args->hexdump = true;
    else if (ft_strcmp(a->av[a->i], "-in") == 0)
	{
		if (get_arg(a, &args->in_file, "-in needs an input file") != 0)
			return (1);
	}
	else if (ft_strcmp(a->av[a->i], "-out") == 0)
	{
		if (get_arg(a, &args->out_file, "-out needs an output file") != 0)
			return (1);
	}
	else if (ft_strcmp(a->av[a->i], "-inkey") == 0)
	{
		if (get_arg(a, &args->inkey_file, "-inkey needs an input key file") != 0)
			return (1);
	}
    else
        return (print_rsautl_usage());
    return (0);
}

int parse_rsautl_args(int ac, char **av)
{
    t_rsautl_args   args;
    t_ac_av      a;

    ft_bzero(&args, sizeof(args));
    a.i = 2;
    a.ac = ac;
    a.av = av;
    while (a.i < a.ac)
    {
        if (a.av[a.i][0] == '-')
        {
            if (rsautl_parse_subargs(&args, &a) != 0)
                return (1);
        }
        else
        {
            ft_fdprintf(2, "Extra operand: '%s'\n", a.av[a.i]);
            return (1);
        }
		a.i++;
    }
    if (args.inkey_file == NULL)
        return (print_error("No keyfile specified"));
    else if (args.pubin == true && args.decrypt == true)
        return (print_error("A private key is needed for this operation"));
    return (rsautl_process(&args));
}
