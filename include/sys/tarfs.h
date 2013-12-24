#ifndef TARFS_H
#define TARFS_H

#include<stdio.h>
#include<defs.h>
#include<my_lib.h>
#include<sys/vfs.h>

/******************* TARFS ******************************/
extern char _binary_tarfs_start;
extern char _binary_tarfs_end;

#define DIRECTORY 5
#define FILE_TYPE 0


struct posix_header_ustar {
	char name[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char checksum[8];
	char typeflag[1];
	char linkname[100];
	char magic[6];
	char version[2];
	char uname[32];
	char gname[32];
	char devmajor[8];
	char devminor[8];
	char prefix[155];
	char pad[12];
};


typedef struct {
	char name[100];
	int size;
	int typeflag;
	uint64_t addr_hdr;
	int par_ind;
} tarfs_entry;

extern tarfs_entry tarfs_fs[100];
extern int tarfs_row_count;

void tarfs_init();
void print_tarfs_file_table();

/************** SYSCALL *******************/
struct file* open_dir_tarfs(char * dir);
dirent*  read_dir_tarfs(struct file * dird);
int close_dir_tarfs(struct file * dird);
uint64_t read_dir_ls(struct file * dir,char *buf,int size);
struct file *open_file_tarfs(char * file);
int read_file_tarfs(struct file *fd, char *buf,  int nbytes);

/****************************************/
void* readTarfile(char *fileName);
struct task_struct* load_elf(char *filename);


//int get_per_ind(char *dir);
//int get_per_ind_file(char *dir);
uint64_t is_file_exists(char* filename);
//uint64_t open_dir(char * dir);
//void print_file_table(tarfs_table_col *table);

//void get_file_sections(char* filename);
uint64_t is_file_exists(char* filename);
void get_file_sections(char* filename);

//TARFS file system




/**************** ELF HEADERS ****************************/

#define ELF_MAGIC 0x464C457F
#define ELF_PROG_LOAD	 1
#define EI_NIDENT 16

typedef struct {
unsigned char e_ident[EI_NIDENT];
uint16_t e_type;
uint16_t e_machine;
uint32_t e_version;
uint64_t e_entry;
uint64_t e_phoff;
uint64_t e_shoff;
uint32_t e_flags;
uint16_t e_ehsize;
uint16_t e_phentsize;
uint16_t e_phnum;
uint16_t e_shentsize;
uint16_t e_shnum;
uint16_t e_shstrndx;
} Elf_hdr;

typedef struct {
uint32_t p_type;
uint32_t p_flags;
uint64_t p_offset;
uint64_t p_vaddr;
uint64_t p_paddr;
uint64_t p_filesz;
uint64_t p_memsz;
uint64_t p_align;
} Elf64_Phdr;

typedef struct {
uint32_t sh_name;
uint32_t sh_type;
uint64_t sh_flags;
uint64_t sh_addr;
uint64_t sh_offset;
uint64_t sh_size;
uint32_t sh_link;
uint32_t sh_info;
uint64_t sh_addralign;
uint64_t sh_entsize;
} Elf64_Shdr;

/*********************************************************/
#endif
