#ifndef _DEFS_H
#define _DEFS_H

#define NULL 0
#define TRUE  1
#define FALSE 0

typedef int bool;
typedef unsigned long __uint64_t;
typedef __uint64_t uint64_t;
typedef unsigned int __uint32_t;
typedef __uint32_t uint32_t;
typedef int __int32_t;
typedef __int32_t int32_t;
typedef unsigned short __uint16_t;
typedef __uint16_t uint16_t;
typedef unsigned char __uint8_t;
typedef __uint8_t uint8_t;
extern uint64_t kernmem1;
extern uint64_t physbase1;
extern uint64_t physfree1;
typedef unsigned long long  size_t;
typedef char 			    sint8_t;
typedef short               sint16_t;
typedef int                 sint32_t;
typedef long long           sint64_t;
typedef uint32_t        __u32;
typedef uint16_t        __u16;
typedef unsigned char   __u8;

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;
extern uint64_t tmp11;
#define	KERNBASE	0xffffffff80000000
#endif
