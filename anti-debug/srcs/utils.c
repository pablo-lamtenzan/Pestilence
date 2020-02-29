/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 18:21:31 by plamtenz          #+#    #+#             */
/*   Updated: 2020/02/29 18:21:31 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

/*
** take matrix of files and the dir path, will check if the content into the dir are files
** if files are founded if will copy and add to files param, if the content is a dir
** it will call itself recursivelly ultil there are only files in the dir (copping the files into files param)
** return matrix of cmdlines
*/
char                **get_cmd_lines(char **files, char *dir_path, char recursion)
{
    struct linux_dirent_x64 *dirent;
    char            *absolute_path;
    char            *file_path;
    char            **folders;
    DIR             *dir;
    int             index;

    if (!dir_path || !(dir = opendir(dir_path) || !files))
        return (NULL);
    while (dirent = readdir(dir)) // or getdents64 need doc
    {
        if (dirent->d_name[0] == '.' || (dirent->d_name[0] == '.' && dirent->d_name[1] == '.'))
            continue ;
        &file_path = get_path(dir_path, dirent->d_name); // if that doesn t woks see how i did it in Famine
        if (is_file(file_path) == FAILURE)
            continue ;
        if (is_directory(file_path) == SUCCESS)
        {
            folders = recursion ? mtrx2d_add_elem(folders, file_path) : folders;
            continue ;
        }
        if (!strcmp(dirent->d_name, "cmdline") && !recursion)
            files = mtrx2d_add_elem(files, file_path);
    }
    index = -1;
    while ((unsigned long)++index < mtrx2d_len(folders))
    {
        if (!(files = get_cmd_lines(files, folders[index], 0)))
            return (NULL);
        (void)free(folders[index]);
    }
    if (folders)
        (void)free(folders);
    if (closedir(dir) == FAILURE)
        return (NULL);
    return (files);
}

/*
** takes an fd and a size, allocates the size and read into the allocation ultil size
** returns the allocated string
*/
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

/*
** open and read 0x400 bytes od data and returns it
*/
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

/*
** trim all without the base name in the file path and return it
*/
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