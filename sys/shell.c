#include <defs.h>
#include <stdio.h>
#include <my_lib.h>
//#include <stdlib.h>
#include <syscall.h>
#include <syscall_dictionary.h>
#include <sys/tarfs.h>
#include <sys/sata.h>
extern char *cursor_pos;
extern char *start_pos;

int readdir_ls_new(struct file *dirp ,char *buf, int size){
	uint64_t dir_desc_addr=(uint64_t)dirp;
	uint64_t buf_addr=(uint64_t)buf;
	uint64_t size_val=(uint64_t)size;
	uint64_t return_value=777;


	if(dirp==NULL){
		printk("\n You are passing NULL directory descriptor");
		return 0;
	}
	if((dirp->tarfs_table_index)<0 || (dirp->tarfs_table_index)>998){
		printk("\n Directory does not exist");
		return 0;
	}
	if(strlen(dirp->filename)==0){
		printk("\n No Directory name mentioned");
		return 0;
	}

	if(dirp->location == FS_LOC)
		__syscall4(FS_READ_DIR_LS,dir_desc_addr,buf_addr,(uint64_t)&size_val,(uint64_t)&return_value);

	else if(dirp->location == TARFS_LOC)
		__syscall4(TARFS_READ_DIR_LS,dir_desc_addr,buf_addr,(uint64_t)&size_val,(uint64_t)&return_value);
	else
		printk("\n Location not Present");

	if(return_value==0)
		printk("\n Directory Empty");
	if(return_value==777)
		printk("\n System call not working");
	return (*(uint32_t *)&return_value);
}


void shell()
{
	
	char shell_buffer[100];
	char *tmp1,*tmp2;
	tmp1=shell_buffer;	
	char *mountpt="/hiphop/";
	int flag=0;
	int cmd_loc=5;
	struct file fd;
	struct file *fd_p;
	 //uint64_t *fd1=(uint64_t *)kmalloc(sizeof(struct file));
          //fd=(struct file*)fd1;
	int i=0;
	char commands[3][10]={"echo","ls","cd"};
	commands[1][2]='\0';
	commands[0][5]='\0';
	commands[2][2]='\0';
	//char *tmp;
	char* tmp3;
	//blacken();
	clear_screen();
	char pwd[40]="";
      pwd[0]='\0';
	int k=3;
	while(1)
	{	
		for(i=0;i<k;i++)
			shell_buffer[i]='\0';
		flag=0;
		cmd_loc=5;
		printk("\n Shell ~%s>",pwd);
		scank("%s",shell_buffer);
		tmp1=shell_buffer;
		for(i=0;i<3;i++)
		{
			tmp2=commands[i];
			if(*tmp1!=*tmp2)
			continue;
			while(*tmp1==*tmp2)
			{
				tmp1++;
				tmp2++;
				//printk("[%c] [%c]",*tmp1,*tmp2);
			}
			if((*tmp2=='\0')&&(*tmp1==' '))
			{
				flag=1;
				tmp1++;
				cmd_loc=i;
			}
			//printk("[i=%d]",i);
		}
		
		if(flag==0)
		{
			printk("command not found");
			continue;
		}
		tmp2=tmp1;
		//printk("%s",tmp2);
             tmp3=mountpt;
	  	while(*tmp1==*tmp3)
		{
			tmp1++;
			tmp3++;
		}
         	flag=0;
		
		if((*tmp1!='\0')&&(*tmp3=='\0') && *(tmp1-1)=='/')
		{
			flag=1;
		}
		switch(cmd_loc)
		{
			case 0:
				//printk("echo encountered");
				break;
			case 1:
				//printk("ls encountered");
				
				strcpy(fd.filename,tmp2);
                            fd.location=flag;//while(1);
				//printk("%s,%d",fd.filename,fd.location);
				for(i=0;i<k;i++)
					shell_buffer[i]='\0';
			//	while(1);
				//readdir_ls(&fd,shell_buffer,10);
				if(flag==1)
				dir_read(&fd,shell_buffer,100);
   				else
				read_dir_ls(&fd,shell_buffer,100);
    				tmp2=shell_buffer;
        			while(*tmp2!='\0')
				{
					if(*tmp2==',')
					*tmp2='\t';
					tmp2++;
				}
				printk("\n%s",shell_buffer);
				//while(1);

				break;
			case 2:
				/*if(strcmp("..",tmp2)==0)
				{	
					if(flag==1)
					tmp=pwd+strlen(pwd)-1;
					else
					tmp=pwd+strlen(pwd)-2;
					tmp1=pwd;

					while(*tmp!='/' || tmp1!=tmp)
                                     tmp--;
    					if(tmp1==tmp)
                                      *tmp='\0';
                                   else {
					tmp++;
                                   tmp='\0';}
				}*/
				//else
				//{
					
					strcpy(fd.filename,tmp2);
					fd.location=flag;
					if(flag==1)
					{ 
					printk("\n [[%d]]",make_dir(fd.filename));
					fd_p=(struct file*)0x1;
					//fd_p=file_open(fd.filename,5);
					if(fd_p==NULL)
					{
						printk("NO directory exists");
						continue;
					}
                                    }
					else
					{
						fd_p=open_file_tarfs(fd.filename);//fd_p=open_dir_tarfs(fd.filename);
						if(fd_p==NULL)
						{
						printk("NO directory exists");
						continue;
						}

					}				
					strcpy(pwd,tmp2);
			
				//}
				
				//fd.location;
				break;

			default:
				printk("BAD command");
		}
		
	}
		
}