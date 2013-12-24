#include <defs.h>

void  _x86_64_asm_isr0();
void  _x86_64_asm_isr1();
void  _x86_64_asm_isr10();
void  _x86_64_asm_isr11();
void  _x86_64_asm_isr14();

extern void isr_create();
extern void interrupt_handler_keyboard(void);
extern void interrupt_handler_timer(void);
extern void timer_interrupt(void);

