#include <defs.h>
#include <stdio.h>
#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/scancodes.h>
#include <sys/kbd.h>
#include <sys/pic.h>
#include <sys/pit.h>
#include <syscall.h>
int buffer_complete=0;
char buffer[512];
char keycode;
extern char return_buffer[512];
extern int clean_buffer;
static int count=0;
extern int start_buffer;
int flag=0;
int countb=0;
void isr_create(){
	fill_idt_entries(32,(uint64_t) timer_interrupt_1, 0);
	fill_idt_entries(33,(uint64_t) _x86_64_asm_isr1, 0);
	fill_idt_entries(42,(uint64_t) _x86_64_asm_isr10, 0);
	fill_idt_entries(43,(uint64_t) _x86_64_asm_isr11, 0);
	fill_idt_entries(14,(uint64_t) _x86_64_asm_isr14, 0);
	fill_idt_entries(0x80,(uint64_t) sys_call_handler,3 );
}

void interrupt_handler_keyboard(void){
	static int shift = 0;
	uint8_t scancode;
	uint8_t code;
	scancode = inb(0x60);
	switch(scancode)
	{
		case KBD_RIGHTSHIFT:
		case KBD_LEFTSHIFT :
			shift = 1;
			break;
		case KBD_RIGHTSHIFT_RELEASE:
		case KBD_LEFTSHIFT_RELEASE :
			shift = 0;
			break;
		default:
			break;
	}
	code=scancode_to_char(scancode,shift);
	if(code != 0)
	{
		printy("key pressed=%c     ",code);
		keycode=code;
		//attaching scanf
			if(start_buffer==1)
			{	
					
				if(clean_buffer==1)
				{	int i=0;
					count=0;
					for(i=0;i<512;i++)
					{
						buffer[i]='\0';
						return_buffer[i]='\0';
					}
					clean_buffer=0;
				}
				if(code!='\n')
				{
					return_buffer[count]=code;
					buffer[count++]=code;
					//countb=count;
					flag=1;
							
				}
				
				//printy("%c",code);	
				if(code=='\n')
				{
					buffer_complete=1;	
				}
				
			}

	}
	PIC_sendEOI(0x20);
}

extern void timer_interrupt(void);

void interrupt_handler_timer(void){
	//static int p=0;
	//	print_timer("Time");
 timer_handler();
//printk("shashank_timer=%d",p);
//	p++;
//timer_interrupt_1();
	PIC_sendEOI(0x20);
}
