#include <defs.h>
#include <stdio.h>
#include <syscall.h>
#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/pic.h>
#include <pmap.h>
#include<syscall_dictionary.h>
#include <sys/tarfs.h> 
#include<sys/sata.h>
#include<task.h>
#include<procsnip.h>

struct sys_call_data s;
extern task_struct* curr;
extern void shell();
extern void timer_interrupt_1();
extern void sleep(int time);
extern int process_instruction;
extern int schedule;
int i=0;
uint64_t collect_address;
void sys_call_handler(void){
struct file* fd;
	

		__asm__ __volatile__("cli");
	__asm__ __volatile__(
			"pushq %rax\n\t"
			"pushq %rbx\n\t"
			"pushq %rcx\n\t"
			"pushq %rdx\n\t"
			"pushq %rsi\n\t"
			"pushq %rdi\n\t"
			"pushq %rbp\n\t"
			"pushq %r8\n\t"
			"pushq %r9\n\t"
			"pushq %r10\n\t"
			"pushq %r11\n\t"
			"pushq %r12\n\t"
			"pushq %r13\n\t"
			"pushq %r14\n\t"
			"pushq %r15\n\t"
			);
	__asm__ __volatile__ (
			"movq %%rax, %0;"
			:"=r"(s.system_call_number)
			:
			:"%rax"
	       );
__asm__ __volatile__(
                   "movq %%rsp, %[curr_kernel_rsp]"
                   : [curr_kernel_rsp] "=m"(curr->kernel_rsp)
                );
	__asm__ __volatile__(
			"movq %%rbx, %0;"
			:"=r"(s.a1)
			:
			:"%rbx"
	       );
	__asm__ __volatile__(
			"movq %%rcx, %0;"
			:"=r"(s.a2)
			:
			:"%rcx"
	       );
	__asm__ __volatile__(
			"movq %%rdx, %0;"
			:"=r"(s.a3)
			:
			:"%rdx"
	       );
	
if(s.system_call_number==25){

process_instruction=3;
sys_fork();
}

if(s.system_call_number==55){

process_instruction=4;
printk("\n EXEC [%c]",(char *)s.a2);
sys_exec((char*)s.a1,s.a2);
}


/********************* FILE SYSTEM + NETWORKING + TARFS ***************************/#define EXIT 0
switch(s.system_call_number){
/*
case SHELL:
	__asm__("sti");
	shell();
	break;

*/
case SLEEP:
	sleep(s.a1);
	break;

case PS:
	getps();
       break;



case EXIT:
	process_instruction=1;
       schedule = 1;
	timer_interrupt_1();
	break;
case PRINTF:
	
	printk((char *)s.a1);

	break;
case MALLOC:
		
   	collect_address=(uint64_t)kmalloc(s.a1);
 	*(uint64_t*)s.a2=collect_address;
	break;
case SCANF :
	__asm__ __volatile__("sti");
	scank((char *)s.a1,(char *)s.a2);
	break;

case CLRSCR :
       clear_screen();
	break;
	

case TARFS_OPEN_DIR:
	fd=open_dir_tarfs((char *)(s.a1));
	if(fd!= NULL)
		(*((struct file *)(s.a2)))=(*(struct file *)fd);
        else 
	s.a2=NULL;
        break;

case TARFS_READ_DIR_LS:
	read_dir_ls(((struct file *)(s.a1)),((char*)s.a2),*((uint32_t *)s.a3)); 
	
        break;

case TARFS_OPEN_FILE :
	fd=open_file_tarfs((char *)(s.a1));
	if(fd!= NULL)
		(*((struct file *)(s.a2)))=(*(struct file *)fd); 
        else
	s.a2=NULL;
        break;

case TARFS_READ_FILE :
	read_file_tarfs(((struct file *)(s.a1)), ((char *)(s.a2)), (*(uint32_t *)(s.a3))); 
        break;

case FS_OPEN_DIR:
	fd=file_open(((char *)(s.a1)),DIRECTORY);
	if(fd!= NULL)
		(*((struct file *)(s.a2)))=(*(struct file *)fd);
	else 
		s.a2=NULL;
	break;

case FS_READ_DIR_LS:
	dir_read(((struct file *)(s.a1)),((char*)s.a2),*((uint32_t *)s.a3)); 
	break;

case FS_OPEN_FILE:
	if((fd=file_open(((char *)(s.a1)),FILE_TYPE))!= NULL)
		(*((struct file *)(s.a2)))=(*(struct file *)fd);
	else 
		s.a2=NULL;
	break;

case FS_READ_FILE:
	file_read(((struct file *)(s.a1)), ((char *)(s.a2)), (*(uint32_t *)(s.a3))); 
        break;

case FS_WRITE_FILE:
	file_write(((struct file *)(s.a1)), ((char *)(s.a2)), (*(uint32_t *)(s.a3))); 
break;

case MAKE_DIR:
	make_dir(((char *)(s.a1))); 
break;

case GETPID:
     (*(uint64_t *)(s.a1)) = (uint64_t)getcurpid();
break;
default:
   printk("\n NO system Calls present");
   
}
//printk("sys call completed successfully");
/********************************************************************************************/
	__asm__ __volatile__(
					"popq %r15\n\t"
					"popq %r14\n\t"
					"popq %r13\n\t"
					"popq %r12\n\t"
					"popq %r11\n\t"
					"popq %r10\n\t"
					"popq %r9\n\t"
					"popq %r8\n\t"
					"popq %rbp\n\t"
					"popq %rdi\n\t"
					"popq %rsi\n\t"
					"popq %rdx\n\t"
					"popq %rcx\n\t"
					"popq %rbx\n\t"
					"popq %rax\n\t"
                                   "popq %rbx\n\t"					);
        __asm__ __volatile__(
                           "sti\n\t"
                             "iretq\n\t"   );
}
