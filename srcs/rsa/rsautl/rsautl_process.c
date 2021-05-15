/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsautl_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 18:26:30 by eduwer            #+#    #+#             */
/*   Updated: 2021/05/15 18:52:56 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl_rsa.h>

static int  continue_rsautl_process(t_rsautl_args *args, t_rsa_key *key)
{
    char    *msg;
    size_t  msg_len;

}

int rsautl_process(t_rsautl_args *args)
{
    char    *inkey;
    size_t  size_inkey;
    t_rsa_key   key;
    t_asn1_conf asn1_conf;
    
    asn1_conf.public = args->pubin;
    asn1_conf.type = PEM;
    asn1_conf.pass = NULL;
    if ((args->fd_in = open(args->inkey_file, O_RDONLY)) == -1)
        return (print_errno("ft_ssl: Can't open input file: "));
    if (read_whole_file(args->fd_in, (void **)&inkey, &size_inkey) != 0)
        return (print_errno("ft_ssl: Error while reading input file: "));
    close(args->fd_in);
    if (asn1_dec_key(inkey, size_inkey, &asn1_conf, &key) != 0)
        return (1);
    free(inkey);
    return (continue_rsautl_process(args, &key));
}