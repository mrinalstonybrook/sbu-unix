#ifndef _SYSCALL_H
#define _SYSCALL_H
#include <defs.h>
#include <stdio.h>

void  _x86_64_asm_syscall();
extern void sys_call_handler(void);
void ps() ;

struct sys_call_data{
uint64_t system_call_number;
uint64_t a1;
uint64_t a2;
uint64_t a3;
uint64_t a4;
uint64_t a5;
};

void _x86_64_asm_raise_interrupt(uint64_t , uint64_t , uint64_t , uint64_t , uint64_t , uint64_t );


#define SYSCALL_PROTO(n) static __inline uint64_t __syscall##n

SYSCALL_PROTO(0)(uint64_t n) {
	uint64_t res;
	__asm__ volatile ("int %1"\
                    :"=a"(res)\
                    :"i"(0x80),"0" (n)\
                    :"cc","memory");
	return res;
}

SYSCALL_PROTO(1)(uint64_t n, uint64_t a1) {
	uint64_t res;
	__asm__ volatile ("int %1"\
                    :"=a"(res)\
                    :"i"(0x80),"0" (n),"b"((uint64_t)(a1))\
                    :"cc","memory");
	return res;
}

SYSCALL_PROTO(2)(uint64_t n, uint64_t a1, uint64_t a2) {
	uint64_t res;
	__asm__ volatile ("int %1"\
                    :"=a"(res)\
                    :"i"(0x80),"0" (n),"b"((uint64_t)(a1)),"c"((uint64_t)(a2))\
                    :"cc","memory");
	return res;
}

SYSCALL_PROTO(3)(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3) {
	return 0;
}

SYSCALL_PROTO(4)(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4) {
	uint64_t res;
    	__asm__ volatile ("int %1"\
                    :"=a"(res)\
                    :"i"(0x80),"0" (n),"b"((uint64_t)(a1)),"c"((uint64_t)(a2)),"d"((uint64_t)(a3)),"D"((uint64_t)(a4))\
                    :"cc","memory");
    	return res;
}


/*
static __inline uint64_t __syscall_prototype(uint64_t system_call_number, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5){
printk("\nIn Syscall \n");
_x86_64_asm_raise_interrupt(system_call_number,a1,a2,a3,a4,a5);
return 1;
}
*/
#endif