#include <defs.h>
#include <stdio.h>
#include <pmap.h>
#include <mm.h>
#include <pagetab.h>

extern char kernmem;


//void open(struct vm_area_struct *area);
//void close(struct vm_area_struct *area);

void alloc_mm_struct(task_struct* taskS)
{
    char* addr = (char*)(taskS+1);
    taskS->mm = (struct mm_struct*)addr;
    taskS->mm->mmap = NULL;
}



struct vm_area_struct * malloc_vma(struct mm_struct* mm)
{
   struct vm_area_struct * vma_tail_struct;
   uint64_t addr;
//check if VMA has already been allocated or not for the memory structure

   if(mm->mmap == NULL)
   {
   //If not allocated, then allocate it a 4KB page
      //addr = (char*)kmalloc(4096);

      void* physaddr=kmalloc_mem();
      addr = PHY_TO_VIRT(physaddr, uint64_t);
      mmap(addr,(uint64_t)physaddr);
      printk("vma allocation came here");

       //while(1);
      vma_tail_struct = (struct vm_area_struct *)addr;
      mm->mmap = vma_tail_struct;
      mm->count++;
      return vma_tail_struct;
    }
   
    else
    {
      vma_tail_struct = mm->mmap;
      //point to the last VMA of the complete list of the VMA
      while(vma_tail_struct->vm_next!=NULL)
      {
         vma_tail_struct = vma_tail_struct->vm_next;
      }

    addr = (uint64_t)((char*)vma_tail_struct+sizeof(struct vm_area_struct));
    /*
    if((ALIGN_DOWN((uint64_t)adr)) == (ALIGN_DOWN((uint64_t)vma_last_struct))) // check if the new vma will fit in the same page
    {
      adr-=sizeof(struct vm_area_struct);
      vma_last->vm_next=(struct vm_area_struct*)adr;
      mm->map_count++;
      return (struct vm_area_struct*)adr;
     }
      else {
      char* adr = (char*)malloc(1); // allocate a new page
      if(!adr) return 0; // out of memory;
    */
      vma_tail_struct->vm_next=(struct vm_area_struct*)addr;
      mm->count++;
      return (struct vm_area_struct*)addr;
     }
}


struct vm_area_struct* populate_vma(elf64_eheader *elf_ehdr, elf64_pheader *elf_phdr)
{
  struct vm_area_struct *vma = (struct vm_area_struct *)kmalloc(sizeof(struct vm_area_struct)); 
  
    vma->vm_start = elf_phdr->p_vaddr;
    vma->vm_end = elf_phdr->p_vaddr + elf_phdr->p_memsz;
    vma->vm_flags = elf_phdr->p_flags;
    vma->vm_pgoff = elf_phdr->p_offset;
    vma->e_header = elf_ehdr;
    vma->p_header = elf_phdr;
	
    
    //vma->grows_down = 0;                   
     //vma->vm_file = (struct file *)kmalloc(sizeof(struct file));
        //vma->vm_file->file_start = (uint64_t)elf_ehdr;
        //vma->vm_file->vm_pgoff = elf_phdr->p_offset;
        //vma->vm_file->vm_sz = elf_phdr->p_filesz;

     vma->vm_next = NULL;
    return vma;

}

void populate_instruction(task_struct* taskS){

if(taskS->mm != NULL)
{
struct vm_area_struct *vma = taskS->mm->mmap; 
int j;
while(vma){
uint64_t temp_viraddr = vma->p_header->p_vaddr;
	kmalloc_virt(temp_viraddr,vma->p_header->p_filesz);
       	
	 for (j=0;j<(vma->p_header->p_filesz);j++)
	   {	    
	      *((char*)(vma->vm_start + j))=*((char*)((uint64_t)vma->e_header + j));
	    }

vma = vma->vm_next;
}
}

}
