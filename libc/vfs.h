#ifndef VFS_INCLUDED
#define VFS_INCLUDED
#include<math.h>
#include<defs.h>
#include<my_lib.h>
/**********TARFS + FS + N/W ************/
#define TARFS_LOC 0 
#define FS_LOC 1 
#define DEFAULT_LOC -1 

#define DEFAULT_TARFS_TABLE_INDEX -1 

#define DIRECTORY 5
#define FILE_TYPE 0
#define DEFAULT_TYPE  9

typedef struct {
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
} file;
typedef file dir;

typedef struct {
	char name[100];
	int type;
	int location;
	int tarfs_table_index;    
	int inode_num;
} dirent;

extern dir kern_vfs_dir;
extern file kern_vfs_fd;
void kern_init_desc();

extern dir vfs_dir;
extern file vfs_fd;
void init_desc();

/**************************** OPERATONS ON TARFS + FS *********************************/
dir *opendir(char * name);
int readdir_ls(dir *dirp);
int closedir(dir *dirp);
file *fopen(char *filename_full_path);
uint32_t fread(file *fd, char *buf,uint32_t size);
int fseek(file *fd, int offset, int whence);
int fwrite(file *fd, char *buf,int size);
int fclose(file *fd);
int make_dir_vfs(char *name);
struct file* malloc(uint64_t size);


/******************************************************************************************/
#endif
