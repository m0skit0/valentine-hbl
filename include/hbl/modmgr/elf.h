#ifndef ELOADER_ELF
#define ELOADER_ELF

#include <common/sdk.h>
#include <common/utils.h>

/*******************/
/* ELF typedefs */
/*******************/
/* Types for ELF file manipulation */
/* Be sure to modify when used on other platform */
typedef void *Elf32_Addr;
typedef int Elf32_Off;
typedef int Elf32_Sword;
typedef int Elf32_Word;
typedef unsigned short int Elf32_Half;
typedef char BYTE;

/*************/
/* ELF types */
/*************/
#define ELF_STATIC 0x0002     /* Static ELF */
#define ELF_RELOC 0xffa0      /* Relocatable ELF, aka PRX */

/**************/
/* ELF HEADER */
/**************/

#define EI_NIDENT 16 //Size of e_ident[]
typedef struct
{
    BYTE e_ident[EI_NIDENT];  //Magic number
    Elf32_Half e_type;      // Identifies object file type
    Elf32_Half e_machine;   // Architecture build
    Elf32_Word e_version;   // Object file version
    Elf32_Addr e_entry;     // Virtual address of code entry
    Elf32_Off e_phoff;      // Program header table's file offset in bytes
    Elf32_Off e_shoff;      // Section header table's file offset in bytes
    Elf32_Word e_flags;     // Processor specific flags
    Elf32_Half e_ehsize;    // ELF header size in bytes
    Elf32_Half e_phentsize; // Program header size (all the same size)
    Elf32_Half e_phnum;     // Number of program headers
    Elf32_Half e_shentsize; // Section header size (all the same size)
    Elf32_Half e_shnum;     // Number of section headers
    Elf32_Half e_shstrndx;  // Section header table index of the entry associated with the
                            // section name string table.
} Elf32_Ehdr;

/* e_ident */
#define EI_MAG0 0    //File identification
#define EI_MAG1 1    //File identification
#define EI_MAG2 2    //File identification
#define EI_MAG3 3    //File identification
#define EI_CLASS 4   //File class
#define EI_DATA 5    //Data encoding
#define EI_VERSION 6 //File version
#define EI_PAD 7     //Start of padding bytes in header (should be set to zero)

/* File class */
#define ELFCLASSNONE 0 //Invalid class
#define ELFCLASS32 1   //32-bit objects
#define ELFCLASS64 2   //64-bit objects

/* Data encoding */
#define ELFDATANONE 0 //Invalid data encoding
#define ELFDATA2LSB 1 //Little-endian
#define ELFDATA2MSB 2 //Big-endian

/* e_type */
#define ET_NONE 0        //No file type
#define ET_REL 1         //Relocatable file
#define ET_EXEC 2        //Executable file
#define ET_DYN 3         //Shared object file
#define ET_CORE 4        //Core file
#define ET_LOPROC 0xff00 //Processor-specific
#define ET_HIPROC 0xffff //Processor-specific

/* e_machine */
#define ET_NONE 0         //No machine
#define EM_M32 1          //AT&T WE 32100
#define EM_SPARC 2        //SPARC
#define EM_386 3          //Intel Architecture
#define EM_68K 4          //Motorola 68000
#define EM_88K 5          //Motorola 88000
#define EM_860 7          //Intel 80860
#define EM_MIPS 8         //MIPS RS3000
#define EM_MIPS_RS4_BE 10 //MIPS RS4000 Big-endian

/* e_version */
#define EV_NONE 0    //Invalid version
#define EV_CURRENT 1 //Current version

/******************/
/* SECTION HEADER */
/******************/

typedef struct
{
    Elf32_Word sh_name;       //Name of section (value is index to string table)
    Elf32_Word sh_type;       //Type of section
    Elf32_Word sh_flags;      //Flags :P
    Elf32_Addr sh_addr;       //Address in process image (0 -> not used)
    Elf32_Off  sh_offset;     //Section offset in file
    Elf32_Word sh_size;       //Section size in bytes
    Elf32_Word sh_link;       //Section header table index link
    Elf32_Word sh_info;       //Extra info
    Elf32_Word sh_addralign;  //Alignment
    Elf32_Word sh_entsize;    //Some sections hold a table of fixed-size entries, such as
                              //a symbol table. This member gives the size of each entry.
} Elf32_Shdr;

/******************/
/* PROGRAM HEADER */
/******************/

typedef struct
{
    Elf32_Word p_type;      // Type of segment
    Elf32_Off p_off;     // Offset for segment's first byte in file
    Elf32_Addr p_vaddr;     // Virtual address for segment
    Elf32_Addr p_paddr;     // Physical address for segment
    Elf32_Word p_filesz;    // Segment image size in file
    Elf32_Word p_memsz;     // Segment image size in memory
    Elf32_Word p_flags;     // Flags :P
    Elf32_Word p_align;     // Alignment
} Elf32_Phdr;

/* p_type */

#define PT_NULL 0
#define PT_LOAD 1               // Loadable segment
#define PT_DYNAMIC 2            // Specifies dynamic linking info
#define PT_INTERP 3             // Location and size of interpreter
#define PT_NOTE 4               // Location and size of additional info
#define PT_SHLIB 5              // Reserved
#define PT_PHDR 6               // Not relevant
#define PT_LOPROC 0x70000000    // Processor-specific semantics
#define PT_HIPROC 0x7fffffff

/*******************/
/* .lib.stub entry */
/*******************/
typedef struct
{
	Elf32_Addr lib_name;     // Pointer to library name
	Elf32_Half import_flags;
	Elf32_Half lib_ver;
	Elf32_Half import_stubs;
	Elf32_Half stub_size;        // Number of stubs imported from library
	Elf32_Addr nid_p;      // Pointer to array of NIDs from library
	Elf32_Addr jump_p;     // Pointer to array of stubs from library
} tStubEntry;

/******************/
/* PRX relocation */
/******************/

// Relocation section type
#define LOPROC 0x700000a0

// Relocation entry
typedef struct
{
    Elf32_Addr r_offset; // Offset of relocation
    Elf32_Word r_info;   // Packed information about relocation
} tRelEntry;

/* Macros for the r_info field */
/* Determines which program header the current address value in memory should be relocated from */
#define ELF32_R_ADDR_BASE(i) (((i)> >16) & 0xFF)

/* Determines which program header the r_offset field is based from */
#define ELF32_R_OFS_BASE(i) (((i)> >8) & 0xFF)

/* Determines type of relocation needed, see defines below */
#define ELF32_R_TYPE(i) (i&0xFF)

/* MIPS Relocation Entry Types */
#define R_MIPS_NONE 0
#define R_MIPS_16 1
#define R_MIPS_32 2
#define R_MIPS_REL32 3
#define R_MIPS_26 4
#define R_MIPS_HI16 5
#define R_MIPS_LO16 6
#define R_MIPS_GPREL16 7
#define R_MIPS_LITERAL 8
#define R_MIPS_GOT16 9
#define R_MIPS_PC16 10
#define R_MIPS_CALL16 11
#define R_MIPS_GPREL32 12

/**************/
/* PROTOTYPES */
/**************/
/* Load static executable in memory using virtual address */
/* Returns total size copied in memory */
int elf_load(SceUID fd, SceOff off, const Elf32_Phdr *phdrs, Elf32_Word phnum,
	void *(* malloc)(const char *name, SceSize, void *));

/* Returns size and address (pstub) of ".lib.stub" section (imports) */
tStubEntry *elf_find_imports(SceUID fd, SceOff off, const Elf32_Ehdr* hdr, size_t *size);

// Get ELF GP value
int elf_get_gp(SceUID fd, SceOff off, const Elf32_Ehdr* hdr, void **buf);


void eboot_get_elf_off(SceUID eboot, SceOff *off);

#endif

