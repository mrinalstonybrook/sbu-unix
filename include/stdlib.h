#ifndef _STDLIB_H
#define _STDLIB_H

#include<defs.h>
int main(int argc, char* argv[], char* envp[]);
void exit(int status);
void sleep(int time);
int getpid();
 void ps();
extern int sleep_time;
/**********TARFS + FS + N/W ************/
#define TARFS_LOC 0 
#define FS_LOC 1 
#define DEFAULT_LOC -1 

#define DEFAULT_TARFS_TABLE_INDEX -1 

#define DIRECTORY 5
#define FILE_TYPE 0
#define DEFAULT_TYPE  9

struct file{
	uint64_t address_tarfs_loc;
	int inode_num;
	int location;
	int perm;
	int size;
	int sector_loc[10];
	int offset;
	int next;
	int tarfs_table_index;    
	char type;
	char filename[100];
} ;

typedef struct {
	char name[100];
	int type;
	int location;
	int tarfs_table_index;    
	int inode_num;
} dirent;



/**************************** OPERATONS ON TARFS + FS *********************************///keepvfs.c
struct file *opendir(char * name);
int readdir_ls(struct file *dirp ,char *buf, int size);
int closedir(struct file *dirp);
struct file *fopen(char *filename_full_path);
uint32_t fread(struct file *fd, char *buf,uint32_t size);
int fseek(struct file *fd, int offset, int whence);
int fwrite(struct file *fd, char *buf,int size);
int fclose(struct file *fd);
int make_dir_vfs(char *name);

int strlen(const char *s);
int strcmp(char *s1, char *s2);
char *  strncpy(char *dst, const char *src, size_t size);
size_t  strlcpy(char *dst, const char *src, size_t size);
int strncmp(const char *s1, const char *s2, uint32_t n);

int strncmp(const char *s1, const char *s2, uint32_t n);
void shell();
void clrscr();


/******************************************************************************************/

void* malloc(uint64_t size_mem);

void user_fork(void);
void exec(char* a,int b);
int atoi(char *str);



#endif
