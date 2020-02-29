/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_tracer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 18:24:43 by plamtenz          #+#    #+#             */
/*   Updated: 2020/02/29 18:24:43 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This function will check if there are tracer tracing
** pestilance process. If the tracer aren't allowed it kills
** pestilance process.
**
** A sort of advanced thread process will be used with this
** function, this function will run in  thread durring ALL
** pestilance execution a will be called each n time checking if
** the process is bein traced: More info ATP in google
*/

char            *get_tracer_status_name(int pid)
{
    int         fd;
    const char  path[64]; // max path size i think
    char        *data;
    char        *addr;
    int         i;

    (void)memset((char *)&path, 0, 63);
    (void)snprintf((char *)&path, 63, "/proc/%d/status", pid);
    if ((fd = syscall(SYS_OPEN, path, O_RDONLY)) == FAILURE)
        return (NULL);
    if ((data = read_file_data(fd, 1024)))
    {
        if ((addr = strstr(data, "Name:")))
            return (NULL);
        i = -1;
        if (addr = addr + sizeof("Name:"))
        {
            while (++i)
                if (i >= strlen(addr) || addr[i] == '\n')
                    break ;
            addr[i] = '\0';
            return (addr);
        }
    }
    return (NULL);
}

void                presistant_tracer_checker(void)
{
    int             pid;
    int             i;
    char            *status_name;
    char            *process;
    char            *basename;
    char            **files;

    files = NULL;
    while (42)
    {
        if (files = get_cmd_lines(files, "/proc", 1))
        {
            while (files[++i])
            {
                process = get_file_data(files[i]);
                basename = get_file_base_name(process);
                if (!stncmp(basename, "Pestilence"))
                {
                    if (pid = get_pid(0))
                    {
                        if ((status_name = get_tracer_status_name(pid))
                                && !allowed_tracer(status_name))
                            (void)kill(pid, SIGKILL);
                    }

                }
                (void)free(process);
                (void)free(basename);
                (void)free(files[i]);
            }
            (void)free(files);
        }
        usleep(1000000);
    }
}
