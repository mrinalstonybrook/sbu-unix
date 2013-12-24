#ifndef _PMAP_H
#define _PMAP_H

#include <defs.h>

void init_freelist(void* physbase, void* physfree);
void set_kernel_in_freelist(uint64_t start_addr, uint64_t end_addr);
void reserve_pages(uint64_t start_addr, uint64_t end_addr);
void* kmalloc_page();
void* kmalloc(uint64_t size_mem);
void* kmalloc_mem();
void remap_bitmap();
void* kmalloc_phy(uint64_t phys_addr, uint64_t size_mem);
void* kmalloc_virt(uint64_t virt_addr,uint64_t size_mem);

struct Page {
	// Next page on the free list.
        struct Page *pp_link;

	// pp_ref is the count of pointers (usually in page table entries)
	// to this page, for pages allocated using page_alloc.
	// Pages allocated at boot time using pmap.c's
	// boot_alloc do not have valid reference count fields.

	uint16_t pp_ref;
};















void* kmalloc_iden(uint64_t size_mem);
#endif
