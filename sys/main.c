#include <defs.h>
#include <stdio.h>
#include <sys/gdt.h>
#include <pagetab.h>
#include <sys/idt.h>
#include <sys/pic.h>
#include <sys/kbd.h>
#include <sys/scancodes.h>
#include <sys/isr.h>
#include <task.h>
#include <sys/tarfs.h>
#include <sys/pci.h>
#include <syscall.h>
#include <sys/ahci.h>
#include<sys/e1000.h>
#include<sys/vfs.h>
#include<sys/sata.h>
#include<my_lib.h>
#include <pmap.h>
#include <mm.h>
#include <procsnip.h>
#include <test.h>

void* kernelbase;
void* kernelfree;
extern task_struct p1;
extern int schedule;

void start(uint32_t* modulep, void* physbase, void* physfree){

	struct smap_t {
		uint64_t base, length;
		uint32_t type;
	}__attribute__((packed)) *smap;

	int i;
	asm volatile("cli");        	
	PIC_remap(0x20, 0x29);
	for(i=0;i<16;i++)
		IRQ_set_mask(i) ;
	IRQ_clear_mask(0);///this is for timer
	IRQ_clear_mask(1);// this is for keyboard
	IRQ_clear_mask(10);
	IRQ_clear_mask(11);
	IRQ_clear_mask(14);
	isr_create();
	reload_idt();
	asm volatile("sti");        
	clear_screen();



	while(modulep[0] != 0x9001) modulep += modulep[1]+2;

	init_freelist(physbase, physfree);
	physfree = kernelfree;
	clear_screen();

	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 /* memory */ && smap->length != 0) {
			printk("Available Physical Memory [%x-%x]\n\n", smap->base, smap->base + smap->length);
			set_kernel_in_freelist(smap->base, smap->base + smap->length);
		}
	}


	uint64_t *pml4 = setup_page_tables((uint64_t)physbase, (uint64_t)physfree);
	printk("pml4 phys addr: %x\n", pml4);
	kernelfree = (void*) 0x500000;  

	pci_init();

	task_struct* t1 = load_elf("bin/hello");
	add_task_to_ready(t1);
	schedule = 1;

	while(1);
}

#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;


void boot(void){
	// note: function changes rsp, local stack variables can't be practically used
	register char *temp1, *temp2;
	__asm__(
			"movq %%rsp, %0;"
			"movq %1, %%rsp;"
			:"=g"(loader_stack)
			:"r"(&stack[INITIAL_STACK_SIZE])
	       );
	reload_gdt();
	setup_tss();
	start(
			(uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
			&physbase,
			(void*)(uint64_t)loader_stack[4]
	     );
	for(
			temp1 = "!!!!! start() returned !!!!!", temp2 = (char*)0xb8000;
			*temp1;
			temp1 += 1, temp2 += 2
	   ) *temp2 = *temp1;
	while(1);
}
