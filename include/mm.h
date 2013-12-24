#ifndef _MM_H
#define _MM_H

#include <defs.h>
#include <task.h>
#include <elf.h>

struct vm_area_struct{
  struct mm_struct *vm_mm;       /* contains a memory structure */
  uint64_t vm_start;              /* this is the start of VMA */
  uint64_t vm_end;                /* this is the end of VMA */
  uint64_t vm_flags;              /* flags */
  struct file * vm_file;          /* mapped file, if any */
  struct vm_area_struct *vm_next;/* list of VMA's */
  uint64_t                vm_pgoff;          /* offset within file */
  uint64_t                vm_memsz;          /* mapped file, if any */
  uint64_t                vm_filestart;
  struct elf64_eheader *e_header;
  struct elf64_pheader *p_header;					
};

struct mm_struct {
  struct vm_area_struct * mmap;  //first vma in the list
  int count;              //number of memory areas                                          
  uint64_t start_code;    //start address of the code                                       
  uint64_t end_code;      //end address of the code                                         
  uint64_t start_data;    //start address of the data                                       
  uint64_t end_data;      //end address of the data                                         
  uint64_t start_brk;     //start address of the heap                                       
  uint64_t brk;           //end address of the heap                                         
  //  uint64_t start_stack;                                                                 
  // uint64_t start_mmap;                                                                   

};

void populate_instruction(task_struct* taskS);

void alloc_mm_struct(task_struct* taskS);  //allocate memory for mm_struct (just after the task structure)
struct vm_area_struct * malloc_vma(struct mm_struct* mm);   //allocating memory for vma 
struct vm_area_struct* populate_vma(elf64_eheader *elf_ehdr, elf64_pheader *elf_phdr);


/*
struct vm_area_struct {
        //struct mm_struct             *vm_mm;        
        unsigned long                vm_start;      
        unsigned long                vm_end;        
        struct vm_area_struct        *next;          
};

struct vm_operations_struct {
        void (*open) (struct vm_area_struct *);
        void (*close) (struct vm_area_struct *);
        struct page * (*nopage) (struct vm_area_struct *, unsigned long, int);
};

*/

//struct vm_area_struct * find_vma(mm_struct *mm, unsigned long addr);

//static inline struct vm_area_struct * find_vma_intersection(struct mm_struct *mm,unsigned long start_addr,unsigned long end_addr);

//unsigned long do_mmap(struct file *file, unsigned long addr,
//                      unsigned long len, unsigned long prot,
//                      unsigned long flag, unsigned long offset);


//int do_munmap(struct mm_struct *mm, unsigned long start, size_t len);

//Also write the system calls for both the do_mmap and do_munmap functions.

//void* malloc_pcb(uint64_t requestsize,void **physfree,uint64_t *pml4);



#endif