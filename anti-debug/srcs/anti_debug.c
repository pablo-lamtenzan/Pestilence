/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anti_debug.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 21:44:56 by plamtenz          #+#    #+#             */
/*   Updated: 2020/02/29 22:41:17 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <anti_debug.h>

t_aes               *anti_debug(char **env, t_atp *atp)
{
    t_aes           *aes;
    
    atp->env = env;
    if (build_atp(atp) == SUCCESS
            && ((aes = init_aes())
            && stop_exection(aes) == FAILURE))
    {
        aes->error = 0;
        return (aes);
    }
    return (NULL);
}