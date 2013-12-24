#ifndef _DEFS_H
#define _DEFS_H
#include<sys/vfs.h>

#define NULL 0

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

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned long QWORD;

#endif
