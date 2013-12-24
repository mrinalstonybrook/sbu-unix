#ifndef _PAGETAB_H
#define _PAGETAB_H

#include <defs.h>
#include <task.h>
#define PR 0x1			//Present
#define RW 0x2			//Read-0/Write-1
#define US 0x4			//User -1/Supervisor -0
#define OS 0x8			// Operating space  	
#define PWT 0x8			//PageWriteThrough
#define PCD 0x10		//PageCahceDisable
#define A 0x20			//Accessed
#define DI 0x40			//Dirty
#define PAT 0x80		//PageAttributeTable
#define G 0x100			//Global
#define NX 0x8000000000000	//NoExecute
#define KFLAGS 1 | 2
#define UFLAGS 1 | 2 | 4
#define COW 0x10000000000000


void copy_page_tables(uint64_t pml4_current, uint64_t pml4_new);

struct PML4
{
	uint64_t entries[512];
};

struct PDP
{
	uint64_t entries[512];
};

struct PD
{
	uint64_t entries[512];
};

struct PT
{
	uint64_t entries[512];
};

/*
#define _PML 0xFFFFFF7FBFDFF000
#define _PDP 0xFFFFFF7FBFFFE000
#define _PD  0xFFFFFF7FFFC01000
#define _PT  0xFFFFFFFF80200000

#define PML4_BASE 0xFFFFFF7FBFDFE000
#define PDP_BASE 0xFFFFFF7FBFC00000 //0xFFFFFEFF7FA00000  0xFFFFFEFF7FBFD000 
#define PD_BASE 0xFFFFFF7FBFDFE000
#define PT_BASE 0xFFFFFF7FBFDFE000

//#define PHY_TO_VIRT_PML4(phy, type) ((type) ((uint64_t) phy + PML4_BASE))
//#define PHY_TO_VIRT_PDP(phy, type) ((type) ((uint64_t) (phy<<12) | PDP_BASE))
//#define PHY_TO_VIRT_PD(phy, type) ((type) ((uint64_t) phy + PD_BASE))
//#define PHY_TO_VIRT_PT(phy, type) ((type) ((uint64_t) phy + PT_BASE))
*/
#define RECURSIVE_SLOT (510L)

#define L4_SHIFT (39)
#define L3_SHIFT (30)
#define L2_SHIFT (21)
#define L1_SHIFT (12)


#define UPPER_ADDR(x) ((uint64_t*)(0xffffL<<48|(x)))

#define PT_ADDR UPPER_ADDR((RECURSIVE_SLOT<<L4_SHIFT))

#define PD_ADDR UPPER_ADDR((RECURSIVE_SLOT<<L4_SHIFT) \
                     |(RECURSIVE_SLOT<<L3_SHIFT))

#define PDP_ADDR UPPER_ADDR((RECURSIVE_SLOT<<L4_SHIFT) \
                     |(RECURSIVE_SLOT<<L3_SHIFT) \
                     |(RECURSIVE_SLOT<<L2_SHIFT))

#define PML4_ADDR UPPER_ADDR((RECURSIVE_SLOT<<L4_SHIFT) \
                     |(RECURSIVE_SLOT<<L3_SHIFT) \
                     |(RECURSIVE_SLOT<<L2_SHIFT) \
                     |(RECURSIVE_SLOT<<L1_SHIFT))

#define PHY_TO_VIRT_PML4_BASE(type) ((type) ((uint64_t) PML4_ADDR))
#define PHY_TO_VIRT_PDP_BASE(virt, type) ((type) ((uint64_t)PDP_ADDR | (uint64_t)(( ((virt<<16)>>16) >>L4_SHIFT)<<L1_SHIFT)))
#define PHY_TO_VIRT_PD_BASE(virt, type) ((type) ((uint64_t) PD_ADDR | (uint64_t)(( ((virt<<16)>>16)>>L3_SHIFT)<<L1_SHIFT)))
#define PHY_TO_VIRT_PT_BASE(virt, type) ((type) ((uint64_t) PT_ADDR | (uint64_t)(( ((virt<<16)>>16)>>L2_SHIFT)<<L1_SHIFT)))


#define PHY_TO_VIRT_PML4(virt, type) ((type) ((uint64_t) PML4_ADDR | (uint64_t) (( ((virt<<16)>>16)>>L4_SHIFT)<<L1_SHIFT)))
#define PHY_TO_VIRT_PDP(virt, type) ((type) ((uint64_t)PDP_ADDR | (uint64_t)(( ((virt<<16)>>16)>>L3_SHIFT)<<L1_SHIFT)))
#define PHY_TO_VIRT_PD(virt, type) ((type) ((uint64_t) PD_ADDR | (uint64_t)(( ((virt<<16)>>16)>>L2_SHIFT)<<L1_SHIFT)))
#define PHY_TO_VIRT_PT(virt, type) ((type) ((uint64_t)virt))

////ANURAG/////////////////
#define USER_PML4_OFFSET (509L)

#define USER_PML4_ADDR  UPPER_ADDR((RECURSIVE_SLOT<<L4_SHIFT) \
                     |(RECURSIVE_SLOT<<L3_SHIFT) \
                     |(RECURSIVE_SLOT<<L2_SHIFT) )

#define USER_PML4_VIRT_BASE_ADDR(type) (type)( (uint64_t)USER_PML4_ADDR | (uint64_t)(USER_PML4_OFFSET<<L1_SHIFT) )

#define USER_PDP_ADDR UPPER_ADDR((RECURSIVE_SLOT<<L4_SHIFT) \
                     |(RECURSIVE_SLOT<<L3_SHIFT))

#define USER_PT_ADDR UPPER_ADDR((USER_PML4_OFFSET<<L4_SHIFT))


#define USER_PD_ADDR UPPER_ADDR((RECURSIVE_SLOT<<L4_SHIFT))


#define USER_PDP_VIRT_BASE_ADDR(virt, type) ((type) ((uint64_t) USER_PDP_ADDR | (uint64_t)(USER_PML4_OFFSET<<L2_SHIFT) | (uint64_t)(( ((virt<<16)>>16)>>39)<<L1_SHIFT)))
#define USER_PD_VIRT_BASE_ADDR(virt, type) ((type) ((uint64_t) USER_PD_ADDR | (uint64_t)(USER_PML4_OFFSET<<L3_SHIFT)  |(uint64_t)(( ((virt<<16)>>16)>>30)<<L1_SHIFT)))
#define USER_PT_VIRT_BASE_ADDR(virt, type) ((type) ((uint64_t) USER_PT_ADDR | (uint64_t)(( ((virt<<16)>>16)>>21)<<L1_SHIFT)))
	



//////////////////



#define PAGE_TABLE_SIZE 512

#define PAGE_SIZE 0x1000

#define PAGE_MAX 65536

#define MAX_PHY_MEM (PAGE_MAX * PAGE_SIZE)

/*** all available physical memory will be mapped to virtual address (physical + VIRT_OFFSET) ***/
#define VIRT_OFFSET 0xFFFFFFFF80000000

#define VIRT_OFFSET_DIFF 0xFFFFDFFF80000000

/*** pointer arithmetic ***/
#define PHY_TO_VIRT(phy, type) ((type) ((uint64_t) phy + VIRT_OFFSET))
#define PHY_TO_VIRT_DIFF(phy, type) ((type) ((uint64_t) phy + VIRT_OFFSET_DIFF))			

#define VIRT_BASE 0xFFFFFFFFF0000000



#define PAGE_ALIGN 0xFFFFFFFFFFFFF000

extern void _cpu_write_cr3(uint64_t val);
extern void _cpu_read_cr2(uint64_t val);

void * setup_page_tables(uint64_t physbase,uint64_t physfree);

struct PT* getPT(struct PML4 *pml4, uint64_t virt_addr);
void mmap(uint64_t virt_addr,uint64_t phy_addr);


uint16_t GetPTIndex(uint64_t lAddress);
uint16_t GetPDIndex(uint64_t lAddress);
uint16_t GetPDPIndex(uint64_t lAddress);
uint16_t GetPML4Index(uint64_t lAddress);

uint64_t get_user_virt_addr(void* phy_addr);
void * setup_user_page_tables(uint64_t kern_physbase, uint64_t kern_physfree);
struct PT* getuserPT(struct PML4 *pml4, uint64_t virt_addr);
uint64_t setup_user_paging();
uint64_t setup_user_paging_p( uint64_t user_pml4);

uint64_t phy_to_virtual(uint64_t phy_addr);

#endif
