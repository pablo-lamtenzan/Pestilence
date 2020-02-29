#ifndef ELF_H
# define ELF_H

#define SUCESS                      0
#define FAILURE                     -1

#define EI_NIDENT                   16
#define MAGIC_SIZE                  4
#define ELF_MAGIC                   "7f454c46" /* [0x7][E][L][F] */
#define ARCH_64                     2
#define ARCH_32                     1
#define L_ENDIAN                    1
#define B_ENDIAN                    2
#define SHT_SYMTAB                  2 /* provides symbols for linking editing */
#define SHT_DYNSYM                  11 /* olds a minimal set f dinamic linking symbols */

/*
**      --> ELF FILE TYPE
*/
#define ELF_OBJ_TYPE                1
#define ELF_EXC_TYPE                2
#define ELF_SHARED_TYPE             3

/*
**      --> SEGMENTS TYPES
*/
#define PT_NULL    			        0
#define PT_LOAD    			        1
#define PT_DYNAMIC 			        2
#define PT_INTERP  			        3
#define PT_NOTE    			        4
#define PT_SHLIB   			        5
#define PT_PHDR    			        6
#define PT_TLS     			        7               /* Thread local storage segment */
#define PT_LOOS    			        0x60000000      /* OS-specific */
#define PT_HIOS    			        0x6fffffff      /* OS-specific */
#define PT_LOPROC  			        0x70000000
#define PT_HIPROC  			        0x7fffffff
#define PT_GNU_EH_FRAME		        0x6474e550

/*
**      --> 64bits elf base types
*/
typedef unsigned long long			Elf64_Addr;
typedef unsigned short 				Elf64_Half;
typedef signed short 				Elf64_SHalf;
typedef unsigned long long 			Elf64_Off;
typedef signed int 					Elf64_Sword;
typedef unsigned int 				Elf64_Word;
typedef unsigned long long 			Elf64_Xword;
typedef signed long long 			Elf64_Sxword;

/*
**      --> 64bits elf header
*/
typedef struct			            elf64_hdr
{
	unsigned char		            e_ident[EI_NIDENT];	/* ELF "magic number" */
	Elf64_Half 			            e_type;				/* Type of file (see ET_* below) */
	Elf64_Half 			            e_machine;			/* Required architecture for this file (see EM_*) */
	Elf64_Word 			            e_version;			/* Must be equal to 1 */
	Elf64_Addr 			            e_entry;	     	/* Entry point virtual address */
	Elf64_Off 			            e_phoff;	     	/* Program header table file offset */
	Elf64_Off 			            e_shoff; 	     	/* Section header table file offset */
	Elf64_Word 			            e_flags;	 		/* Processor-specific flags */
	Elf64_Half 			            e_ehsize;		 	/* Size of ELF header, in bytes */
	Elf64_Half 			            e_phentsize;		/* Size of an entry in the program header table */
	Elf64_Half 			            e_phnum;      		/* Length OF Program segment header. */
	Elf64_Half 			            e_shentsize;		/* Size of an entry in the section header table */
	Elf64_Half			            e_shnum;			/* Number of entries in the section header table */
	Elf64_Half			            e_shstrndx;			/* Sect hdr table index of sect name string table */
}						            Elf64_Ehdr;

/*
**      --> program segment header
*/
typedef struct			            elf64_phdr
{
	Elf64_Word			            p_type;			// Type of segment
	Elf64_Word			            p_flags;		// Segment flags
	Elf64_Off			            p_offset;		// File offset where segment is located, in bytes
	Elf64_Addr			            p_vaddr;		// Virtual address of beginning of segment
	Elf64_Addr			            p_paddr;		// Physical addr of beginning of segment (OS-specific)
	Elf64_Xword			            p_filesz;		// Num. of bytes in file image of segment (may be zero)
	Elf64_Xword			            p_memsz;		// Num. of bytes in mem image of segment (may be zero)
	Elf64_Xword			            p_align;		// Segment alignment constraint
}						            Elf64_Phdr;

/*
   sh_size       This member holds the section's size in bytes.  Unless the
                 section type is SHT_NOBITS, the section occupies sh_size
                 bytes in the file.  A section of type SHT_NOBITS may have a
                 nonzero size, but it occupies no space in the file.

   sh_offset     This member's value holds the byte offset from the
                 beginning of the file to the first byte in the section.
                 One section type, SHT_NOBITS, occupies no space in the
                 file, and its sh_offset member locates the conceptual
                 placement in the file.

   e_shnum       This member holds the number of entries in the section
                 header table.  Thus the product of e_shentsize and
                 e_shnum gives the section header table's size in bytes.
                 If a file has no section header table, e_shnum holds the
                 value of zero.
                 If the number of entries in the section header table is
                 larger than or equal to SHN_LORESERVE (0xff00), e_shnum
                 holds the value zero and the real number of entries in
                 the section header table is held in the sh_size member of
                 the initial entry in section header table.  Otherwise,
                 the sh_size member of the initial entry in the section
                 header table holds the value zero.

   sh_name       This member specifies the name of the section.  Its value
                 is an index into the section header string table section,
                 giving the location of a null-terminated string.
*/

typedef struct			            elf64_shdr
{
	Elf64_Word			            sh_name;       	/* Section name, index in string tbl */
	Elf64_Word			            sh_type;       	/* Type of section */
	Elf64_Xword			            sh_flags;     	/* Miscellaneous section attributes */
	Elf64_Addr			            sh_addr;       	/* Section virtual addr at execution */
	Elf64_Off			            sh_offset;      /* Section file offset */
	Elf64_Xword			            sh_size;      	/* Size of section in bytes */
	Elf64_Word			            sh_link;       	/* Index of another section */
	Elf64_Word			            sh_info;       	/* Additional section information */
	Elf64_Xword			            sh_addralign; 	/* Section alignment */
	Elf64_Xword			            sh_entsize;   	/* Entry size if section holds table */
}						            Elf64_Shdr;

/*
**      --> ELF MANAGEMENT STRUCTS
*/
typedef struct                      s_segment
{
    Elf64_Phdr                      *data;
    int                             index;
    struct s_segment                *next;
    struct s_segment                *prev;
}                                   t_segment;

typedef struct                      s_section
{
    Elf64_Shdr                      *data;
    char                            *name;
    void                            *content;
    struct s_section                *next;
    struct s_section                *prev;
}                                   t_section;

typedef struct                      s_elf
{
    char                            *filename;
    Elf64_Ehdr                      *hdr;
    unsigned char                   magic[MAGIC_SIZE * 2 + 1];
    char                            b_endian;
    char                            l_endian;
    size_t                          size;
    void                            *buff;
}                                   t_elf;

/*
**      --> ELF PARSE, SANITIZE, WRITE METHODS
*/

#endif