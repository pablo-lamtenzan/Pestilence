/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_tracer_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 19:26:15 by plamtenz          #+#    #+#             */
/*   Updated: 2020/02/29 19:40:21 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "persistent_tarcer.h"

char                *get_file_base_name(const char *path)
{
    int             i;
    char            *base_name;

    i = 0;
    while (path[i++]);
    i++;
    while (--i > 0)
        if (path[i] == '/')
        {
            i++;
            break ;
        }
    i = i < 0 ? 0 : i;
    if (!(base_name = strdup(path + i)))
        return (NULL);
    return (base_name);
}

char                *get_file_data(const char *filename)
{
    int             fd;
    struct stat     buff;
    char            *data;

    if (syscall(SYS_STAT, filename, &buff) == FAILURE)
        return (strdup(""));
    if ((fd = syscall(SYS_OPEN, filename, O_RDONLY) == FAILURE))
        return (strdup(""));
    if (!(data = read_file_data(fd, 0x400)))
        return (strdup(""));
    if (syscall(SYS_CLOSE, fd) == FAILURE)
        return (strdup(""));
    return (data);
}

char                *read_file_data(int fd, size_t size)
{
    int             read_bytes;
    char            *buff;

    if (!(buff = calloc(sizeof(char), size + 1)))
        return (NULL);
    if ((read_bytes = syscall(SYS_READ, fd, buff, size)) == FAILURE)
    {
        (void)free(buff);
        return (NULL);
    }
    return (buff);
}

int             get_pid(char is_parent)
{
    int         fd;
    int         bytes_read;
    char        *addr;
    char        *buff[1024];

    if ((fd = syscall(SYS_OPEN, "/proc/self/status", O_RDONLY)) == -1)
        return (FAILURE);
    if ((bytes_read = syscall(SYS_READ, fd, buff, sizeof(buff) - 1)) > 0)
        if ((addr = strstr(buff, is_parent ? "PPid:" : "TracerPid:")))
            return (atoi(addr + (is_parent ? sizeof("PPid:") : sizeof("TracerPid:")) - 1));
    return (FAILURE);
}

char            allowed_tracer(t_tracer *tracer)
{
    const char  *tracers[] = ALLOWED_TRACERS;
    int         i;

    i = -1;
    while (tracers[++i])
        if (!strcmp(tracer->cmdline, tracers[i])
                || !strcmp(tracer->status_name, tracers[i]))
            return (SUCCESS);
    return (FAILURE);
}