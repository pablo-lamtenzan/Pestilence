/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tracers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 18:21:14 by plamtenz          #+#    #+#             */
/*   Updated: 2020/02/29 18:21:14 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


typedef struct		s_tracer
{
	char			*cmdline;
	char			*status_name;
	char			*status;
}					t_tracer;

/*
** check if the dir exist
*/
char            dir_check(int pid)
{
    const char  path[64]; // max path size
    struct stat buff;

    (void)menset((char *)&path, 0, 63);
    (void)snprintf((char *)&path, 63, "/proc/%d", pid);
    return (!stat((char *)&path, &buff) && S_ISDIR(buff.st_mode) == 1);
}

/*
** returns the pid of the tracer of the process or of the parent process
*/
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

/*
** put into struct trace given as param cmdline, status_name and status values
*/
char            get_tracer(t_tracer *tracer, int pid)
{
    int         fd;
    const char  path[64]; // max path size i think
    char        *data;
    char        *addr;
    int         i;

    (void)memset((char *)&path, 0, 63);
    (void)snprintf((char *)&path, 63, "/proc/%d/cmdline", pid);
    if ((fd = syscall(SYS_OPEN, path, O_RDONLY)) == FAILURE)
        return (FAILURE);
    tracer->cmdline = read_file_data(fd, 1024);
    if (syscall(SYS_CLOSE, fd) == FAILURE)
        return (FAILURE);
    (void)bzero(path, sizeof(path));
    (char *)&path = NULL;
    (void)memset((char *)&path, 0, 63);
    (void)snprintf((char *)&path, 63, "/proc/%d/status", pid);
    if ((fd = syscall(SYS_OPEN, path, O_RDONLY)) == FAILURE)
        return (FAILURE);
    if ((data = read_file_data(fd, 1024)))
    {
        tracer->status = data;
        if ((addr = strstr(data, "Name:")))
            return (FAILURE);
        i = -1;
        if (addr = addr + sizeof("Name:"))
        {
            while (++i)
                if (i >= strlen(addr) || addr[i] == '\n')
                    break ;
            addr[i] = '\0';
            tracer->status_name = addr;
        }
    }
    return (SUCCESS);
}



/*
** check is current tracers tracked are allowed
*/
#define ALLOWED_TRACERS     {"bash", "zsh", "csh", "sh", "dash", "ksh93", "tcsh", "fish", "rbash", "ksh", 0}

char            allowed_tracer(t_tracer *tracer)
{
    const char  *tracers[] = ALLOWED_TRACERS;
    int         i;

    i = -1;
    while (tracers[++i])
        if (!strcmp(tracer->cmdline, tracer[i])
                || !strcmp(tracer->status_name, tracer[i]))
            return (SUCCESS);
    return (FAILURE);
}

/*
** check is the program is bein traced by not allowed tracer
*/
char            check_tracers(t_aes *eas)
{
    int         pid;
    t_tracer    tracer;
    if (((pid = get_pid(0)) > 0 || (pid = get_pid(1)) > 0)
            && dir_check(pid)
            && (get_tracer(&tracer, pid) == SUCCESS))
    {
        if (allowed_tracer(&tracer))
            aes->error = 0;
        if (tracer.status)
            (void)free(tracer.status);
        if (tracer.cmdline)
            (void)free(tracer.status);
        return (SUCCESS);
    }
    return (FAILURE);
}