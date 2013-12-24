#include <defs.h>
#include <stdio.h>
#include <sys/gdt.h>
#include <sys/pagetab.h>
#include <sys/idt.h>
#include <sys/pic.h>
#include <sys/kbd.h>
#include <sys/scancodes.h>
#include <sys/isr.h>
#include <task.h>
#include <my_lib.h>
#include <math.h>
int start_buffer=0;
extern int buffer_complete;
extern char* buffer;
extern char keycode;
int clean_buffer=0;
char return_buffer[512];
extern int countb;
extern int flag;


void scank(char * a, char * b){
start_buffer=1;
//printk(" ");
int i=0;
char* cursor=buffer;
int cursor_position=0;
int prev_position=-1;

while(buffer_complete==0)
{
	//wait for the buffer to complete
	i++;
	if(cursor==0)
	{
		//cursor=buffer;
	}
	while(cursor!='\0')
	{
		cursor++;
		cursor_position++;
	}
	if(prev_position!=cursor_position)
	{
		//if(buffer!=0)
		//printk("%s",&buffer);
		if(flag==1)
		printk("%c",keycode);
		flag=0;
		prev_position=cursor_position;
		//prev_position=countb;
	}
	
	//buffer_complete=buffer_complete;
	//printk("\twaiting");
}
//while(1);
start_buffer=0;
buffer_complete=0;
clean_buffer=0;
//start copying the buffer to the required variable for now printk
//printk("read from scanf: %s",return_buffer);
//cursor=&buffer;
int* int_pointer=(int*)b;
int temp=atoi(return_buffer);
char* add=(char*)b;
int j=0;
switch(a[1])
{
	case 'd':
		*int_pointer=temp;
		printk("hello !!:%d,%d",temp,*int_pointer);	
		clean_buffer++;
		break;
	case 'c':
		*add=return_buffer[0];
		printk("\nthe character value is:%c",*add);
		clean_buffer++;
		break;
	case 's':
		
		for(j=0;j<512;j++)
		{
			//printk("Copying values in b %x",b);
			*(b+j)=return_buffer[j];	
			if(return_buffer[j]=='\0')	
			{
				break;
			}
		}
		//printk("Copying values in b %s",b);

		//while(1);

		//b=return_buffer;
		//clean buffer here
		clean_buffer++;
		break;

	
}

}