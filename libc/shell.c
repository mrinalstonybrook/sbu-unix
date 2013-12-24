#include<syscall.h>
#include<stdio.h>
#include <defs.h>
#include<stdlib.h>
//__syscall0(14);
char *cursor_pos=(char *) 0xB8000;
char *start_pos=(char *) 0xB8000;
char * strcpy(char *dst, const char *src);
int strcmp(char *s1, char *s2)
{
       while (*s1 == *s2++) {
          if (*s1++ == 0){ 
               return (0);
                 }
            }
        return (*(unsigned char *) s1 - *(unsigned char *) --s2);
}

char *
strcpy(char *dst, const char *src) {
	char *ret;

	ret = dst;
	while ((*dst++ = *src++) != '\0')
		/* do nothing */;
        *dst='\0';
	return ret;
}


char shell_buffer[512];

void  blacken()
{
	char *start_tmp=start_pos;
	int i=0;
	while(i<80*25*2)
	{
		*start_tmp=0;
		start_tmp=start_tmp+1;
		i++;
	}
}

void shell()
{
	//__asm__("sti");
	//while(1);	
	char dup_buffer[512];
	char* bufp=shell_buffer;
	char commandbuf[50];
	char commandparameters[512];
	//char *commandp=commandbuf;
	int number_of_commands=10;
	int commandno=-1;
	int compflag=0;
	char *mountpt="/hiphop/";
	int j=0;	
	
	char commands[10][512]={"echo","ls"};
	
	__syscall0(11);
	//while(1);
	//setting the cursor to start of the screen
//	int c=()
	//cursor_pos=start_pos;//(char *) 0xB8000;
	//printk("\n");
	int cmp=0;
	//blacken();
	int i=0;
	//while(1);
	for(i=0;i<512;i++)
		{
			//shell_buffer[i]='\0';
			//dup_buffer[i]='\0';
			dup_buffer[i]=dup_buffer[i];
		}

	while(1)
	{	
		//__asm__ __volatile__("cli");

		//printf("\n\nShell>");
		
		//__asm__ __volatile__("sti");
		//while(1);
		scanf("%s",bufp);
		while(1);
		//__asm__ __volatile__("cli");

		
		cmp=cmp;
		//if(cmp==0)
		//printk("%d",getpid());
		//printk("\n%d",cmp);
		
		for(i=0;i<512;i++)
		{
			//shell_buffer[i]='\0';
			dup_buffer[i]=shell_buffer[i];
			//dup_buffer[i+1]='\0';
		}
		for(i=0;i<50;i++)
		{
			commandbuf[i]=dup_buffer[i];
			if(dup_buffer[i+1]==' ')
			{
				commandbuf[i+1]='\0';
				break;
			}
		}
		i=i+2;
		for(j=0;j<512;j++)
		{
			commandparameters[j]=dup_buffer[i++];
		}
		cmp=strcmp("ls",dup_buffer);
		//if(cmp==0)	
		//printk("\npid %d",getpid());
		while(1);
		printf("\necho from shell:%s",shell_buffer);
		printf("\nthe command: %s",commandbuf);
		printf("\n the commandparameters:%s",commandparameters);
		for(i=0;i<number_of_commands;i++)
		{
			if(strcmp(commands[i],commandbuf)==0)
			{
				commandno=i;
			}
		}
		switch(commandno)
		{
			case 0:
				printf("echo encountered");
				break;
			case 1:
				printf("ls encountered");
				struct file fd;
				strcpy(fd.filename,commandparameters);
				for(i=0;i<8;i++)
				{
					if(commandparameters[i]!=mountpt[i])
					{
						break;
					}
					if(i==7)
					{
						compflag=1;
					}
					//||mountpt[i+1]=='/'
				}
				if(compflag==1)
				{
					fd.location=1;
					compflag=0;
					printf("finally!!");
					char tmp[512];
					readdir_ls(&fd,tmp,10);
					printf("%s",tmp);
				}
				else
				{
					fd.location=0;
				}
				
				break;
			default:
				printf("BAD command");
		}
		//printk("\nShell>");
		//__asm__ __volatile__("sti");

	}
		
}
