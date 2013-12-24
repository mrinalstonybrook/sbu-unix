#include <defs.h>

extern void fill_idt_entries(int isr_num,uint64_t  isr_address,int dpl);
extern void reload_idt();
extern uint64_t idt[];

