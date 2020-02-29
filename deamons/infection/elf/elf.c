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

/*
** returns a new elf entity dinamically allocated and initialisated
*/
t_elf                   *new_elf(void)
{
    t_elf               *new;

    if (!(new = malloc(sizeof(t_elf))))
        return (NULL);
    new = (t_elf *){ .hdr=NULL, .arch_x64=0, .arch_x32=0,
            .l_endian=0, .b_endian=0, .size=0, .buff=NULL, .filename=NULL};
    return (new);
}

/*
** free the elf entity given as argument
*/
void                    free_elf(t_elf *elf)
{
    if (elf->hdr)
        (void)free(elf->hdr);
    if (elf->filename)
        (void)free(elf->filename);
    if (elf->buff)
        (void)syscall(SYS_MMAP, elf->buff, elf->size);
    (void)free(elf);
}

/*
** parse elf file and puts into its elf struct the architecture of this file
*/
void                    get_bit_architecture(t_elf *elf)
{
    const int           arch = elf->hdr->e_ident[4];
    if (arch == ARCH_64)
        elf->arch_x64 = 1;
    else if (arch == ARCH_32)
        elf->arch_x32 = 1;
}

/*
** parse elf file and puts into its elf struct the endian
*/
void                    get_endian(t_elf *elf)
{
    const int           endian = elf->hdr->e_ident[5];
    if (endian == B_ENDIAN)
        elf->b_endian = 1;
    else if (endian == L_ENDIAN)
        elf->l_endian = 1;
}

/*
** parse elf file and puts into its elf struct the magic value
*/
void                    get_magic(t_elf *elf)
{
    char                *tmp;
    char                *start;
    int                 index;

    if (!(tmp = malloc(sizeof(char) * MAGIC_SIZE * 2)))
        return ;
    start = tmp;
    (void)bzero(elf->magic, MAGIC_SIZE * 2);
    index = -1;
    // works but i can try to it by another way
    while (++index < MAGIC_SIZE)
    {
        (void)asprintf(&tmp, "%x", elf->hdr->e_ident[index]);
        (void)strncat((char *)elf->magic, tmp, 2);
        tmp += 2;
    }
    free(start);
}

/*
** return true is the actual elf entity is a elf file
*/
char                    is_elf_file(t_elf *elf)
{
    return (!strcmp(ELF_MAGIC, (char *)elf->magic) ? SUCESS : FAILURE);
}

/*
** init and get the header for the current elf file
*/
char                    get_elf_header(t_elf *elf)
{
    if (!elf->buff)
        return (FAILURE);
    const Elf64_Ehdr    *hdr = (Elf64_Ehdr *)elf->buff;
    if ((elf->hdr = malloc(sizeof(Elf64_Ehdr))))
        return (FAILURE);
    (void)bzero(elf->hdr, sizeof(Elf64_Ehdr));
    (void)memcpy(elf->hdr, hdr, sizeof(Elf64_Ehdr));
    (void)get_magic(elf);
    (void)get_bit_architecture(elf);
    (void)get_endian(elf);
    return (SUCCESS)
}

/*
** takes and elf entity and a filename path and parse
*/
char                   *read_elf(t_elf *elf, const char *filename)
{
    int                 fd;
    uint64_t            filesize;

    if (!(elf = new_elf()) || !filename)
        return (FAILURE);
    if ((fd = syscall(SYS_OPEN, filename, O_RDONLY | O_NONBLOCK)) == FAILURE)
        return (FAILURE);
    if ((filesize = syscall(SYS_LSEEK, fd, 0, SEEK_END) <= 0))
    {
        (void)syscall(SYS_CLOSE, fd);
        return (FAILURE);
    }
    elf->size = filesize;
    if ((elf->buff = (void *)syscall(SYS_MMAP, NULL, filesize,
            PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
    {
        (void)syscall(SYS_CLOSE, fd);
        return (FAILURE);
    }
    if (syscall(SYS_LSEEK, fd, 0, SEEK_SET) < 0)
    {
        (void)syscall(SYS_CLOSE, fd);
        (void)free_elf(elf);
        return (FAILURE);
    }


    /* find pattern ??? */

    elf->filename = strdup(filename);
    (void)get_elf_header(elf);
    if (is_elf_file(elf) && elf->arch_x64)
    {
        if (syscall(SYS_CLOSE, fd) == FAILURE)
            return (FAILURE);
        return (SUCCESS);
    }
    (void)syscall(SYS_CLOSE, fd);
    (void)free_elf(elf);
    return(FAILURE);
}

/*
** return the addr where all the elf->buff is cpy
*/
void                *get_elf_content_addr(t_elf *elf)
{
    void            *addr;

    if (!(addr = (void *)malloc(sizeof(char) * elf->size)))
        return (FAILURE); // null better
    (void)bzero(addr, elf->size);
    memcpy(addr, elf->buff, elf->size);
    return (addr);
}