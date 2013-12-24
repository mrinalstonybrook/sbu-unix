#ifndef PAGETAB_H
#define PAGETAB_H
#include <defs.h>
#define P1		1				// Present(1)
#define RW		2				// Read(0)/Write(1)
#define US		4				// User(1)/Supervisor(0)
#define PWT		8				// Page-Level Writethrough
#define PCD 	16				// Page-Level Cache Disable
#define A		32				// Accessed(1)
//#define D		64 				// Dirty(1)
#define PAT 	128				// Page Attribute Table (or sometimes Page Size, but always 0 for our purposes
#define G		256				// Global
#define	NX32	0x80000000
#define NX		0x8000000000000	// No execute

// VMA section
struct vma{
	//mm_struct_p mm_struct_process;
	uint64_t s_addr;
	uint64_t e_addr;
	struct vma * next;
};
struct mm_struct{
	int pid;
	struct vma *vma_list;
};

void* malloc_pcb(uint64_t requestsize,void **physfree,uint64_t *pml4);

///////////////////////////

//typedef  vma vma_t;
//typedef vma * vma_p;
 
struct PML4e {
	uint64_t value;
};

struct PDPE {
	uint64_t value;
};

struct PDE {
	uint64_t value;
};

struct PTE {
	uint64_t value;
};

struct PML4 {
	struct PML4e entries[512];
};

struct PDP {
	struct PDPE entries[512];
};

struct PD {
	struct PDE entries[512];
};

struct PT {
	struct PTE entries[512];
};
void * getFreePage();
uint64_t newgetFreePage();
//uint64_t getFreePage();
void *set_kernel_pt();
int GetPTIndex(uint64_t lAddress);
int GetPDIndex(uint64_t lAddress);
int GetPDPIndex(uint64_t lAddress);
int GetPML4Index(uint64_t lAddress);
void print_index(uint64_t * tmp);
void make_paging_entry(struct vma * vma1,uint64_t * pml4);
void interrupt_handler_pagefault(void);
#endif
