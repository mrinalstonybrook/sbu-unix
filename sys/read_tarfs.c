#include <defs.h>
#include <stdio.h>
#include <math.h>
#include <tarfs.h>
#include <elf.h>
#include <pmap.h>
#include <mm.h>
#include <pagetab.h>
#include <my_lib.h>
#include <task.h>



void* readTarfile(char *file_name) {

    struct posix_header_ustar * header = (struct posix_header_ustar*)(&_binary_tarfs_start);
    uint64_t  size_of_file = 0; 
    
    do {
        /*convert header->size string to octal to integer*/
        size_of_file = octalToDecimal((uint64_t)atoi(header->size));
        //printf("seriously!!");
        if(strcmp(file_name, header->name) == 0)
        {   
            printk("File Name: %s..File Size: %d bytes..Contents of file: ", header->name, size_of_file);
	     return (void *)header;

        }
        
        if(size_of_file > 0) 
            header = header + 1 + (size_of_file / 513 + 1);
        else
            header = header + 1;
    } while(header < (struct posix_header_ustar*)&_binary_tarfs_end);

return NULL;    
}


void populateTask(elf64_eheader *elf_ehdr, elf64_pheader *elf_phdr,struct task_struct *tsk)
{
	if(tsk->mm == NULL)
		return;

    struct vm_area_struct *vma = populate_vma(elf_ehdr, elf_phdr);
    /*
		     PF_X   An executable segment.
                   PF_W   A writable segment.
                   PF_R   A readable segment.
    */

    if(elf_phdr->p_flags == (PF_R + PF_X) )	//data
    {
    tsk->mm->start_code = elf_phdr->p_vaddr;
    tsk->mm->end_code = elf_phdr->p_vaddr + elf_phdr->p_memsz;

	}
    else if(elf_phdr->p_flags == (PF_R + PF_W + PF_X) ){	//text
	    tsk->mm->start_data = elf_phdr->p_vaddr;
	    tsk->mm->end_data = elf_phdr->p_vaddr + elf_phdr->p_memsz;

	}

    if(tsk->mm->mmap == NULL)   
        tsk->mm->mmap = vma;        
    else {
	struct vm_area_struct *curr = tsk->mm->mmap;
	 while(curr->vm_next != NULL)
		curr = curr->vm_next;
        curr->vm_next = vma;       
    }

    vma->vm_mm = tsk->mm;

};




struct task_struct* load_elf(char *filename)
{
	__asm__ __volatile__("cli");

    struct elf64_eheader *elf_ehdr;
    struct elf64_pheader *elf_phdr;
    struct task_struct *tsk;
    struct posix_header_ustar *elf_header = (struct posix_header_ustar *)readTarfile(filename);

    if(elf_header == NULL)
        return NULL;
	
    elf_ehdr = (struct elf64_eheader *)(elf_header+1);
    elf_phdr = (struct elf64_pheader *)((uint64_t)elf_ehdr + elf_ehdr->e_phoff);
	
    tsk = kmalloc(sizeof(struct task_struct));
    void* virt_addr = kmalloc(sizeof(struct mm_struct));
    tsk->mm = virt_addr;
    //tsk = kmalloc_page_random();

    crt_new_user_prs_stack(tsk, (void*)elf_ehdr->e_entry);
	   // while(1);
    //tsk->rip = elf_ehdr->e_entry;   //entry point address of .text section
    tsk->kernel_rsp =  (void *)kmalloc(PAGE_SIZE*5) ;	 //allocate a blank page in kernel for stack
    tsk->mm->mmap = NULL;            

    elf_phdr = (struct elf64_pheader *)((uint64_t)elf_ehdr + elf_ehdr->e_phoff);
    int i=0;
	//while(1);
    for (i = 0; i < elf_ehdr->e_phnum; ++i) {
        if(elf_phdr->p_type == PT_LOAD) {
                printk("elf header type: PT_LOAD\n");
                populateTask(elf_ehdr, elf_phdr, tsk);
        }
        elf_phdr++;
    }
__asm__ __volatile__("sti");
 
   return tsk;
}


