/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 18:21:09 by plamtenz          #+#    #+#             */
/*   Updated: 2020/02/29 18:21:09 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <anti_debug.h>

char            check_process(char **process)
{
    char        **cmdlines;
    char        *tmp;
    char        *save;
    int         i;
    int         j;

    cmdlines = NULL;
    i = -1;
    if (cmdlines = get_cmd_lines(cmdlines, "/proc", 1))
    {
        while (cmdlines[++i])
        {
            tmp = get_file_data(cmdlines[i]);
            save = tmp;
            tmp = get_file_base_name(tmp);
            j = -1;
            while (process && process[++j])
                if (!strcmp(tmp, process[j]))
                    return (SUCCESS);
            (void)free(tmp);
            (void)free(save);
            (void)free(cmdlines[i]);
        }
        (void)free(cmdlines);
    }
    return (FAILURE);
}
