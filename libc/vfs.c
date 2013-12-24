#include<defs.h>
#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
#include<syscall_dictionary.h>
int strncmp(const char *s1, const char *s2, uint32_t n)
{
  for ( ; n > 0; s1++, s2++, --n)
    if (*s1 != *s2)
      return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
    else if (*s1 == '\0')
      return 0;
  return 0;
}
int strlen(const char *s) {
        int n;

        for (n = 0; *s != '\0'; s++)
                n++;
        return n;
}

char mount_fs[10]="/hiphop/";


/***************FILESYSTEMS + TARFS *************************/

struct file *opendir(char * name){

	char *tmp;
        uint64_t *fd=(uint64_t *)malloc(sizeof(struct file));
	uint64_t name_addr=(uint64_t)name;
	uint64_t dir_addr;
	dir_addr=(uint64_t)fd;

	if((strncmp(name, "/hiphop/", 8)) == 0 ){
		printf("\n ITS IN FILE SYSTEM");
		tmp=name;
		name_addr=(uint64_t)tmp;
		__syscall2(FS_OPEN_DIR,name_addr,dir_addr);
		goto error;
	}
	__syscall2(TARFS_OPEN_DIR,name_addr,dir_addr);

error:
	if(dir_addr == NULL)
		return NULL;

	return (struct file *)fd;


}

int readdir_ls(struct file *dirp ,char *buf, int size){
	uint64_t dir_desc_addr=(uint64_t)dirp;
	uint64_t buf_addr=(uint64_t)buf;
	uint64_t size_val=(uint64_t)size;
	uint64_t return_value=777;


	if(dirp==NULL){
		printf("\n You are passing NULL directory descriptor");
		return 0;
	}
	if((dirp->tarfs_table_index)<0 || (dirp->tarfs_table_index)>998){
		printf("\n Directory does not exist");
		return 0;
	}
	if(strlen(dirp->filename)==0){
		printf("\n No Directory name mentioned");
		return 0;
	}

	if(dirp->location == FS_LOC)
		__syscall4(FS_READ_DIR_LS,dir_desc_addr,buf_addr,(uint64_t)&size_val,(uint64_t)&return_value);

	else if(dirp->location == TARFS_LOC)
		__syscall4(TARFS_READ_DIR_LS,dir_desc_addr,buf_addr,(uint64_t)&size_val,(uint64_t)&return_value);
	else
		printf("\n Location not Present");
	if(buf[0]!='\0')
		return size;
	if(return_value==0)
		printf("\n Directory Empty");
	if(return_value==777)
		printf("\n no directory exits");
	return (*(uint32_t *)&return_value);
}


int closedir(struct file *dirp){
	dirp=0x0;
	return 1;

}



struct file *fopen(char *filename_full_path){

	char *tmp;
       uint64_t *fd=(uint64_t *)malloc(sizeof(struct file));

	uint64_t name_addr=(uint64_t)filename_full_path;
	uint64_t file_addr;
	file_addr=(uint64_t)fd;

	if((strncmp(filename_full_path,"/hiphop/", 8)) == 0 ){
		printf("\n ITS IN FILE SYSTEM");
		tmp=filename_full_path;
		name_addr=(uint64_t)tmp;
		__syscall2(FS_OPEN_FILE,name_addr,file_addr);
		goto error;
	}

	__syscall2(TARFS_OPEN_FILE,name_addr,file_addr);

error:
	if(file_addr == NULL)
		return NULL;
	return (struct file *)fd;
}

uint32_t fread(struct file *fd, char *buf,uint32_t size){
	uint64_t file_desc=(uint64_t)fd;
	uint64_t buf_addr=(uint64_t)buf;
	uint64_t size_val=(uint64_t)size;
	uint64_t return_val=0;

	if(fd->location == FS_LOC)
		__syscall4(FS_READ_FILE,file_desc,buf_addr,(uint64_t)&size_val,(uint64_t)&return_val);
	else if(fd->location == TARFS_LOC)
		__syscall4(TARFS_READ_FILE,file_desc,buf_addr,(uint64_t)&size_val,(uint64_t)&return_val);
	else
		printf("\n Location not Present");

	return (*(uint32_t *)&return_val);
}

int fseek(struct file *fd, int offset, int whence){
	if(fd == NULL)
		return 0;
	int new_pos = offset + whence;
	if(new_pos < 0 || fd->size < new_pos)
		return 0;
	fd->offset = offset + whence;
	return 1;
}

int fwrite(struct file *fd, char *buf,int size){

	uint64_t file_desc=(uint64_t)fd;
	uint64_t buf_addr=(uint64_t)buf;
	uint64_t size_val=(uint64_t)size;
	uint64_t return_val=0;

	if(fd->location == FS_LOC)
		__syscall4(FS_WRITE_FILE,file_desc,buf_addr,(uint64_t)&size_val,(uint64_t)&return_val);
	else if(fd->location == TARFS_LOC){
		printf("\nNo write for tarfs Location");
		__syscall4(TARFS_READ_FILE,file_desc,buf_addr,(uint64_t)&size_val,(uint64_t)&return_val);
	}
	else
		printf("\n Location not Present");
	if(buf[0]=='\0')
	{
			return size;
	}
	return 0;
}

int fclose(struct file *fd){
	
	fd=0x0;
	return 1;
}

int make_dir_vfs(char *name){
	uint64_t name_addr=(uint64_t)name;
	uint64_t return_val=0;
	if((strncmp(name, mount_fs, strlen(mount_fs))) == 0 ){
		printf("\n ITS IN FILE SYSTEM");
		__syscall2(MAKE_DIR,name_addr,(uint64_t)&return_val);
	}
	return (*(uint32_t *)&return_val);
}

/**********************************************************/


