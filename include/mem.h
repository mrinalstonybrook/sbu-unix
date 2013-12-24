#ifndef _MEM_H
#define _MEM_H

#define UserCode       0x0000000000300000
#define VAddr          0x0000000000000000
#define PageSize	  0x1000
#define PageMap        0x0000000000242000
#define TOTAL_PAGE     32768

void create_freelist(long* avl_blocks,long physbase,long physfree);
//void* InitMemManagement();
#endif