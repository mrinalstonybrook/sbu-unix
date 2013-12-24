#include <defs.h>
#include <my_lib.h>
#include <tarfs.h>
#include<math.h>

tarfs_entry tarfs_fs[100];
int tarfs_row_count;
dirent dentd;

struct file kern_vfs_dir;
struct file kern_vfs_fd;

void kern_init_desc(){
	int i=0;
	kern_vfs_dir.address_tarfs_loc=0x0;
	kern_vfs_fd.address_tarfs_loc=0x0;

	kern_vfs_dir.inode_num=0;
	kern_vfs_fd.inode_num=0;

	kern_vfs_dir.location=DEFAULT_LOC;
	kern_vfs_fd.location=DEFAULT_LOC;

	kern_vfs_dir.perm=0;
	kern_vfs_fd.perm=0;

	kern_vfs_dir.size=0;
	kern_vfs_fd.size=0;

	for(i=0; i<10;i++){
		kern_vfs_dir.sector_loc[i]=0;
		kern_vfs_fd.sector_loc[i]=0;
	}

	kern_vfs_dir.offset=0;
	kern_vfs_fd.offset=0;

	kern_vfs_dir.next=0;
	kern_vfs_fd.next=0;

	kern_vfs_dir.tarfs_table_index=0;
	kern_vfs_fd.tarfs_table_index=0;

	kern_vfs_dir.type=DEFAULT_TYPE;
	kern_vfs_fd.type=DEFAULT_TYPE;

	kern_vfs_dir.filename[0]='\0';
	kern_vfs_fd.filename[0]='\0';
}

int get_per_ind(char *dir){
    char name[100];
    int len = strlen(dir);
    strcpy(&name[0], dir);
    len = len-2;
    while(name[len] != '/'){
        len--;
        if(len == 0)
            return 999;
    }
    name[++len] = '\0';
    int i = 0;
    while(strcmp(&name[0], &(tarfs_fs[i].name[0])) !=  0)
        i++;
    return i;
}

int get_per_ind_file(char *dir){
	char name[100];
	int len = strlen(dir);
	strcpy(&name[0], dir);
	len = len-1;
	while(name[len] != '/'){
		len--;
		if(len == 0)
			return 999;
	}
	name[++len] = '\0';
	int i = 0;
	while(strcmp(&name[0], &(tarfs_fs[i].name[0])) !=  0)
		i++;
	return i;
}

uint64_t is_file_exists(char* filename){
	struct posix_header_ustar *test_tarfs = (struct posix_header_ustar *)&_binary_tarfs_start;
	int i = 1, temp = 512;
	uint64_t size;
	while(strlen(test_tarfs->name) != 0){
		test_tarfs = (struct posix_header_ustar *)(&_binary_tarfs_start + temp);
		size = octalToDecimal(atoi(test_tarfs->size));
		//printk("\n");//printk(test_tarfs->name);//printk("    size %d", size);
		if(strlen(test_tarfs->name) == 0)
			return 999;
		if(strcmp(test_tarfs->name, filename) >= 0)
			return temp + 512;
		if(size == 0)
			temp = temp + 512;
		else
			temp +=  (size%512==0) ? size + 512 : size + (512 - size%512) + 512;
		i += 1;
	}
	return 0;
}
/************ TARFS INITIALISATION ******************/
void tarfs_init(){
	struct posix_header_ustar *tarfs_var = (struct posix_header_ustar *)&_binary_tarfs_start;
	int i = 0, temp = 0;
	uint64_t size;
	tarfs_row_count=0;
	tarfs_entry tarfs_e;
	while(1){
		tarfs_var = (struct posix_header_ustar *)(&_binary_tarfs_start + temp);
		if(strlen(tarfs_var->name) == 0){
			tarfs_row_count=i-1;
			break;
		}

		size = octalToDecimal(atoi(tarfs_var->size));
		strcpy(&tarfs_e.name[0], tarfs_var->name);
		tarfs_e.size = size;
		tarfs_e.addr_hdr = (uint64_t)&_binary_tarfs_start + temp;
		tarfs_e.typeflag = atoi(tarfs_var->typeflag);
		
              if(tarfs_e.typeflag == FILE_TYPE)
			tarfs_e.par_ind = get_per_ind_file(&(tarfs_e.name[0]));
		else if(tarfs_e.typeflag == DIRECTORY){
			tarfs_e.par_ind = get_per_ind(&(tarfs_e.name[0]));
		}



		tarfs_fs[tarfs_row_count++] = tarfs_e;
	//	//printk(&(tarfs_fs[i].name[0])); 
	//	//printk("   I[%d]     P[%d] \n", i, tarfs_fs[i].par_ind);
		i++;
		if(size == 0)
			temp = temp + 512;
		else
			temp +=  (size%512==0) ? size + 512 : size + (512 - size%512) + 512;
	}

}
void print_tarfs_file_table(){
        int i=0;
        printk("\n [name] [index] [parent_index]");
        while(i<=tarfs_row_count){
                printk("\n[%s] [%d] [%d]",tarfs_fs[i].name,i,tarfs_fs[i].par_ind);
              //  //printk("\n[%s] [%d] [%d] [%x] [%d]",tarfs_fs[i].name,tarfs_fs[i].size,tarfs_fs[i].typeflag,tarfs_fs[i].addr_hdr,tarfs_fs[i].par_ind);
                i++;
        }
}


/*************************************************/

struct file* open_dir_tarfs(char * dir){
	kern_init_desc();
	tarfs_entry tarfs_e;
	int i = 0;
	while(1){
		tarfs_e = tarfs_fs[i];
		i++;
		if(strlen(tarfs_e.name) == 0)
			break;

		if(strcmp(&(tarfs_e.name[0]), dir) == 0 && tarfs_e.typeflag == DIRECTORY){
			strcpy(kern_vfs_dir.filename,dir); 
			kern_vfs_dir.size=tarfs_e.size;
			kern_vfs_dir.type=tarfs_e.typeflag;
			kern_vfs_dir.address_tarfs_loc=tarfs_e.addr_hdr;
			kern_vfs_dir.tarfs_table_index= i-1;
			kern_vfs_dir.offset = 0;
			kern_vfs_dir.location = TARFS_LOC;
			kern_vfs_dir.next = 0;
			return &kern_vfs_dir;
		}
	}
	printk("\n No such directory ");
	return NULL;
}

dirent*  read_dir_tarfs(struct file * dird){
    if(dird->next == -1){
        //printk("\nInvalid directory pointer.");
        return NULL;
    }
    tarfs_entry tarfs_e;
    int i = 0, parent = -1;
    int child = 0;
    char *dir = dird->filename;
    while(1){
        tarfs_e = tarfs_fs[i];
        if(strlen(tarfs_e.name) == 0)
            break;
        
        if( strcmp(&(tarfs_e.name[0]), dir) == 0){
            parent = i;
            i++; 
            continue; 
         }
         if((strncmp(&(tarfs_e.name[0]), dir, strlen(dir)) == 0 ) &&( tarfs_e.par_ind == parent)){
            if(dird->next == child){
                strcpy(dentd.name, tarfs_e.name);
                dentd.type = tarfs_e.typeflag;
                dentd.tarfs_table_index = i-1;
                dird->next = ++child;
                return &dentd;
            }
            child++;
         }
        i++;
    }
    return NULL;
}

int  close_dir_tarfs(struct file * dird){
        strcpy(dird->filename, "");
        dird->address_tarfs_loc = 0;
        dird->next = -1;
        dird->tarfs_table_index = -1;
        return 1;
}
/*
uint64_t read_dir_ls(char * dir){
    tarfs_entry tarfs_e;
    int i = 0, parent = -1;
    while(1){
        tarfs_e = tarfs_fs[i];
        if(strlen(tarfs_e.name) == 0)
            break;
        
        if(strcmp(&(tarfs_e.name[0]), dir) == 0){
            parent = i;i++; continue; 
         }
         if((strncmp(&(tarfs_e.name[0]), dir, strlen(dir)) == 0 ) &&( tarfs_e.par_ind == parent)){
            //printk(tarfs_e.name + strlen(dir));//printk("\n");
         }
        i++;
    }
    return 0;
}
a*/


uint64_t read_dir_ls(struct file * dir,char *buf,int size){
	int i=0,k=0;
	int par_ind = -1;
       
	for(i=0;i<tarfs_row_count;i++){
		if(strcmp(dir->filename,tarfs_fs[i].name)==0){
			par_ind=i;
			break;
		}
	}
	for(i=0;i<tarfs_row_count;i++){
		if(tarfs_fs[i].par_ind==par_ind){
   	  	  	 if(k>size-5)
				{
					buf[k-1]='\0';
					return 1;


				}
			strcpy(buf+k,tarfs_fs[i].name);
                        k=k+strlen(tarfs_fs[i].name);
                        buf[k]=',';
                        k=k+1;
		}
	}
	buf[k-1]='\0';
return 1;

}

struct file *open_file_tarfs(char * file){
	kern_init_desc();
	tarfs_entry tarfs_e;
	int i = 0;
	while(1){
		tarfs_e = tarfs_fs[i];
		i++;
		if(strlen(tarfs_e.name) == 0)
			break;

		if(strcmp(&(tarfs_e.name[0]), file) == 0 && tarfs_e.typeflag == FILE_TYPE){
			strcpy(kern_vfs_fd.filename, file); 
			kern_vfs_fd.size=tarfs_e.size;
			kern_vfs_fd.type=tarfs_e.typeflag;
			kern_vfs_fd.address_tarfs_loc=tarfs_e.addr_hdr;
			kern_vfs_fd.tarfs_table_index= i-1;
			kern_vfs_fd.offset = 0;
			kern_vfs_fd.location = TARFS_LOC;
			return &kern_vfs_fd;
		}
	}   
	printk("\n No such file ");
	return NULL;
}


int read_file_tarfs(struct file *fd, char *buf,  int nbytes){
	//struct posix_header_ustar* file_hdr = (struct posix_header_ustar *) (fd->address_tarfs_loc); 
	//printk("\n");
	//printk(file_hdr->name);
	//int file_size =  octalToDecimal(atoi(file_hdr->size));
	//printk ("\n [addr] %x [size] %d", fd->address_tarfs_loc, file_size);
	//printk ("\n nbytes + offset %d", (nbytes + fd->offset));
	char * tmp =(char *)buf;
	char * file_start_addr = (char *)(fd->address_tarfs_loc + 512 + fd->offset);
	//printk("\nsize %d file %x", nbytes, file_start_addr);
	int i = 0;
	while(nbytes-- > 0){
		tmp[i++] = *file_start_addr++;
	}    
	tmp[i]='\0';
	//while(1);

	//printk("\n");
	////printk(tmp);
	return nbytes;
}
/*
int close_file_tarfs(uint64_t file_addr)
{
    strcpy(file_desc.filename, "");
    file_desc.inode_num = -1;
    file_desc.offset = -1;
    file_desc.perm = -1;
    file_desc.size = -1;
    file_desc.type = -1;
    int i = 0;
    for(i=0; i<10; i++)
        file_desc.sector_loc[i] = 0;
    return 1;
}



*/
/******************************************************/
