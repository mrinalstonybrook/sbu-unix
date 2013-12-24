#include <elf.h>
#include <defs.h>
#include <stdio.h>
#include <pagetab.h>
#include <my_lib.h>
#include <pmap.h>
#include <mm.h>
#include <task.h>
/*From the data of an ELF executable, determine how its segments
 * need to be loaded into memory.
 * @param exeFileData buffer containing the executable file
 * @param exeFileLength length of the executable file in bytes
 * @param exeFormat structure describing the executable's segments
 * and entry address; to be filled in
 * @return 0 if successful, < 0 on error
 */

  
task_struct elfTask; 
struct mm_struct mmstr;


int elfDebug=1;
uint64_t temp_viraddr;

int Parse_ELF_Executable(char *exeFileData, uint64_t exeFileLength,
			 struct Exe_Format *exeFormat , elf64_pheader * phdr)
{
  task_struct* taskS; 
  elf64_eheader *hdr;
  int i;
  
  hdr = (elf64_eheader *) exeFileData;
    printk(" here is e_phnum  %d: %x:",hdr->e_phnum,hdr->e_phnum);
	
    printk("\n Inside ELF: [  %x: %x: %x: \n %x: %x: %x:",hdr->e_type,hdr->e_version,hdr->e_entry,hdr->e_phoff,hdr->e_phnum,hdr->e_shnum);
	 

  if (exeFileLength < sizeof(elf64_eheader) ||
      strncmp(exeFileData, "\x7F" "ELF", 4) != 0) {
    if (elfDebug)
      printk("Not an ELF executable\n");
    return 1;
  }

  if (hdr->e_phnum > EXE_MAX_SEGMENTS) {
    if (elfDebug)
      printk("Too many segments (%d) in ELF executable\n", hdr->e_phnum);
    return 1;
  }

  if (exeFileLength < hdr->e_phoff + (hdr->e_phnum * sizeof(elf64_pheader))) {
    if (elfDebug)
      printk("Not enough room for program header\n");
    return 1;
  }


   printk("\nGetting from Elf:%d",hdr->e_phnum);
  exeFormat->numSegments = hdr->e_phnum;
  exeFormat->entryAddr = hdr->e_entry;

  phdr = (elf64_pheader *) (exeFileData + hdr->e_phoff);

  printk("\n Phdr address %x  Num of Segments %x Entry address %x ",phdr,hdr->e_phnum,hdr->e_entry);



  //  printk("\n Phdr address %x ",pdr);
  int p1 =0;
  //  printk("\n Num Segment of Exe Format : %d", exe_format->numSegments);
 
 for(;p1< hdr->e_phnum; p1++){
       printk("\n Segment details used : e_type %x :flags %x : offset %x: vaddr %x : paddr %x : filesize %x : memsize %x : allignment %x ",
                                  phdr[p1].p_type,phdr[p1].p_flags,phdr[p1].p_offset,
                                   phdr[p1].p_vaddr,phdr[p1].p_paddr,(phdr[p1].p_filesz),
                                   phdr[p1].p_memsz,phdr[p1].p_align);
  }
    
 // task structure for elf task
     taskS = &elfTask;
     // creating process stacks
     //crt_new_user_prs_stack( taskS,(void *)hdr->e_entry);
     taskS->mm = &mmstr;
     taskS->mm->mmap = NULL;
 
for(i=0;i<hdr->e_phnum;i++)
   {
     printk("my e_type is : %d",phdr->p_type);
     int chk = phdr->p_type;
     
     if (chk==1)
       {
	 printk("We need to load at this location %x\n",phdr->p_vaddr);
	 
	 printk("memz %d\n",phdr->p_memsz);
	 temp_viraddr = phdr->p_vaddr;
	 void* physaddr=kmalloc_mem();
        mmap(temp_viraddr,(uint64_t)physaddr);
	memset((void *)(temp_viraddr & PAGE_ALIGN), 0, PAGE_SIZE);
	
	 //alloc_vpage(elfph->p_vaddr);
        printk("the value at new location [%x]", *(uint64_t*)temp_viraddr);
	 int j=0;
     
     uint64_t address=0;    
     address++;
	 struct vm_area_struct* vma = malloc_vma(taskS->mm);
	 vma->vm_mm=taskS->mm;
	 vma->vm_start = phdr->p_vaddr;
	 vma->vm_end = (uint64_t)((phdr->p_vaddr+phdr->p_memsz));
	 address = vma->vm_end;
	 vma->vm_memsz = phdr->p_memsz;
	 vma->vm_pgoff = phdr->p_offset;
	 vma->vm_next = NULL;
	 //vma->vm_filestart = (uint64_t)((uint64_t)hdr + hdr->e_phoff);
       vma->vm_filestart = (uint64_t)hdr;
	 vma->vm_flags=(uint64_t)(phdr->p_flags);
	 printk("vma start %x %x",vma->vm_start,vma->vm_filestart);	
	 printk("\nmmemz %x  \n %x",phdr->p_memsz,phdr->p_filesz); 
       //while(1);       
	 for (j=0;j<phdr->p_filesz;j++)
	   {	    
	      *((char*)(vma->vm_start + j))=*((char*)(vma->vm_filestart + j));
	    }
       }
     phdr= phdr + 1;
 }
//while(1);
	uint64_t abc=0;
	printk("\ngoing to switch %x",abc);
	//printk("\nswitching to address %x\n",taskS->mm->mmap->vm_start);
	printk("\nswitching to address %x\n",hdr->e_entry);

//while(1);
	__asm__ __volatile__(
			"movq %1, %%rax;"
			"callq %1;"
			:"=g"(abc)
			:"r"(hdr->e_entry)
	       );

while(1);

////////////////
  for (i = 0; i < hdr->e_phnum; ++i) {
    struct Exe_Segment *segment = &exeFormat->segmentList[i];
    printk("\n reading structure %d: \n",i);
    segment->offsetInFile = phdr[i].p_offset;
    segment->lengthInFile = phdr[i].p_filesz;
    segment->sizeInMemory = phdr[i].p_memsz;
     printk("\n Segments:  number :%d Offset %x : Length in file %x: Size in memory %x: ",i,segment->offsetInFile,segment->lengthInFile , segment->sizeInMemory);
 

    if (segment->lengthInFile > segment->sizeInMemory) {
      if (elfDebug)
	                printk
			  ("Segment %d: length in file (%lu) exceeds size in memory (%lu)\n",
			   i, segment->lengthInFile, segment->sizeInMemory);
      return 1;
    }
  }



return 0;

}


void print_exe_format(struct Exe_Format* exe_format){

  int i =0;
  printk("\n Num Segment of Exe Format : %d", exe_format->numSegments);
  for(;i<exe_format->numSegments; i++){

    printk("\n Segments: %d : %d: %d: %d",exe_format->segmentList[i].offsetInFile,exe_format->segmentList[i].lengthInFile,exe_format->segmentList[i].startAddress,exe_format->segmentList[i].sizeInMemory );
  }



}

void print_p_format(struct Exe_Format* exe_format,elf64_pheader* pdr){

  printk("\n Phdr address %x ",pdr);
  int i =0;
  printk("\n Num Segment of Exe Format : %d", exe_format->numSegments);
  for(;i<exe_format->numSegments; i++){

    //    printk("\n Segments: %x : %x: %x: %x :%x :%x :%x : %x ",pdr[i].e_type,pdr[i].offset,pdr[i].vaddr,pdr[i].paddr,pdr[i].fileSize,pdr[i].memSize,pdr[i].flags,pdr[i].alignment);
  }

  /*
  e_typedef struct {
    unsigned int e_type;
    unsigned int offset;
    unsigned long vaddr;
    unsigned long paddr;
    unsigned long fileSize;
    unsigned long memSize;
    unsigned long flags;
    unsigned long alignment;
 
    */


}
/*
// Set up the initial program binary, stack, and processor flags
// for a user process.
// This function is ONLY called during kernel initialization,
// before running the first user-mode environment.
//
// This function loads all loadable segments from the ELF binary image
// into the environment's user memory, starting at the appropriate
// virtual addresses indicated in the ELF program header.
// At the same time it clears to zero any portions of these segments
// that are marked in the program header as being mapped
// but not actually present in the ELF file - i.e., the program's bss section.
//
// All this is very similar to what our boot loader does, except the boot
// loader also needs to read the code from disk. Take a look at
// boot/main.c to get ideas.
//
// Finally, this function maps one page for the program's initial stack.
//
// load_icode panics if it encounters problems.
// - How might load_icode fail? What might be wrong with the given input?
//
static void load_elf_binary(task *t, char *filename){
   // Hints:
   // Load each program segment into virtual memory
   // at the address specified in the ELF section header.
   // You should only load segments with ph->p_e_type == ELF_PROG_LOAD.
   // Each segment's virtual address can be found in ph->p_va
   // and its size in memory can be found in ph->p_memsz.
   // The ph->p_filesz bytes from the ELF binary, starting at
   // 'binary + ph->p_offset', should be copied to virtual address
   // ph->p_va. Any remaining memory bytes should be cleared to zero.
   // (The ELF header should have ph->p_filesz <= ph->p_memsz.)
   // Use functions from the previous lab to allocate and map pages.
   //
   // All page protection bits should be user read/write for now.
   // ELF segments are not necessarily page-aligned, but you can
   // assume for this function that no two segments will touch
   // the same virtual page.
   //
   // You may find a function like region_alloc useful.
   //
   // Loading the segments is much simpler if you can move data
   // directly into the virtual addresses stored in the ELF binary.
   // So which page directory should be in force during
   // this function?
   //
   // You must also do something with the program's e_entry point,
   // to make sure that the environment starts executing there.
   // What? (See env_run() and env_pop_tf() below.)

   // LAB 3: Your code here.

      elf64_eheader *elf = (Elf_hdr *)((char *)open(filename)+ 512);

       elf64_pheader *ph, *eph;
       // struct Page *p = NULL;
       ph = (elf64_pheader *) ((uint8_t *) elf + elf->phoff);

       eph = ph + elf->e_phnum;

       for (; ph < eph; ph++) {
           if (ph->e_e_type == ELF_PROG_LOAD) {
               //int64_t va = ph->vaddr;
               // uint64_t size = ph->memSize;
               //uint64_t offset = ph->offset;
               //uint64_t i = 0;
               if (ph->fileSize > ph->memSize)
                   print("Wrong size in elf binary\n");
               print("\n vaddr : %x size %x", ph->vaddr, ph->memSize);

               region_alloc(t, (void*) ph->vaddr, ph->memSize);
               // Switch to env's address space so that we can use memcpy
               lcr3(t->cr3);
		 
	 struct vm_area_struct* vma = malloc_vma(taskS->mm);
	 vma->vm_mm=process_struct->mm;
	 vma->vm_start = elfph->p_vaddr;
	 vma->vm_end = (uint64_t)((elfph->p_vaddr+elfph->p_memsz));
	 address = vma->vm_end;
	 vma->vm_memsz = elfph->p_memsz;
	 vma->vm_pgoff = elfph->p_offset;
	 vma->vm_next = NULL;
	 vma->vm_filestart = (uint64_t)((uint64_t)elf + elf->e_phoff);
	 vma->vm_flags=(uint64_t)(elfph->p_flags);
	 printk("vma start %p",vma->vm_start);

	 printk("mmemz %p",elfph->p_memsz); 
               memcpy((char*) ph->vaddr, (void *) elf + ph->offset,
                       ph->fileSize);
	
		 elfph= elfph + 1;
  printk("count is %d",count);
		struct vm_area_struct* vma2 = malloc_vma(process_struct->mm); 


vma2->vm_mm = process_struct->mm;
vma2->vm_start = ALIGN_DOWN(address + 0x1000);  
vma2->vm_end = vma2->vm_start;
vma2->vm_memsz = 0;
vma2->vm_pgoff = 0;
vma2->vm_next = NULL;
vma2->vm_filestart = NULL;
vma2->vm_flags = 0|VM_READ|VM_WRITE;

vma2 = malloc_vma(process_struct->mm);
vma2->vm_mm = process_struct->mm;
vma2->vm_start = (uint64_t) (process_struct->user_stack);
vma2->vm_end = (uint64_t) (process_struct->user_stack) + process_struct->user_stack_size;
vma2->vm_memsz = 0;
vma2->vm_pgoff = 0;
vma2->vm_next = NULL;
vma2->vm_filestart = NULL;
vma2->vm_flags = 0|VM_READ|VM_WRITE|VM_AUTO_GROW;
 



 asm volatile("movq %0,%%cr3"::"b"(temp_cr3));
 add(&task_list,(uint64_t) process_struct);
 asm volatile("sti");
	
               if (ph->fileSize < ph->memSize)
                   memset((char*) ph->vaddr + ph->fileSize, 0, ph->memSize
                           - ph->fileSize);

               // Switch back to kernel's address space
               lcr3(boot_cr3);
           }
       }

       // Now map one page for the program's initial stack
       // at virtual address USTACKTOP - PGSIZE.
       //region_alloc(e, (void*) (USTACKTOP - PGSIZE), PGSIZE);

       // Magic to start executing environment at its address.
       t->e_entry = elf->e_e_entry;
   }

}*/