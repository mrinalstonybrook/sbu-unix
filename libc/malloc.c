#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>
int a=0;
void* malloc(uint64_t size)
{
	//int a=0;
	uint64_t a=0x0;
	__syscall2(12,(uint64_t)size,(uint64_t)&a);
	return (void*)a;
}